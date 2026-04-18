#include <Arduboy2.h>
#include "image.h"
#include "Font3x5.h"
#include "ServersScreen.h"
#include "State.h"
#include "commonUi.h"
#include "OfficeScreen.h"
#include "SettingsScreen.h"
#include "gameLogic.h"
#include "music.h"
#include <ArduboyTones.h>
#include "companion.h"
#include "splashScreen.h"

ArduboyTones sound(arduboy.audio.enabled);

Arduboy2 arduboy;
Font3x5 font3x5 = Font3x5();

/*

TODO:

- muzyka
- usunąc komunikat claudii o overload, teraz jest zabezpieczneie przed kupnem, ewentualnie odblokować moliwosć kupna po komunikacie
- zmienić komunikat przed kupnem, na informujacy o braku wystarczającej ilosci serwerw zamiast o overload
- włączyć autosave by default
- usunać erase z ustawień, dodać opcje exit/logout, ktra będzie wychodzić do ekranu startowego
- gdy autosave jest włączone nie pokazyać opcji save w menu
- [x] gdy upgrade daję duo u/m powyej 1k nie mieści się w kafelku
- [x] po wczytaniu save, staty serwera są błędne i zanione dopiero po zakupie upgrade'u lub serwera, przeliczają się na poprawne
- ai context, moze mieć odrobiny większy boost aby przyśpieszyć zbieranie kasy na kolejne upgrade
- [x] zakup kazdego ai upgrade oblokowuje ostatni finałowy upgrade, niekoniecznie wymaxowany level
- [x] endgame nie działa
*/


void setup() {
  arduboy.begin();
  arduboy.setFrameRate(30);
  recalculateStats();

  arduboy.audio.begin();
  // temporary disable music, because it is really annoying, and I want to focus on making the game fun first
  // music = arduboy.audio.enabled();
  music = false;

  if (music) {
    // sound.tones(music_loop);
  }
}


void loop() {
  if (!arduboy.nextFrame()) return;
  
  if (!splashScreen()) {
    return;
  }

  // every x frames, update the game state
  if (arduboy.everyXFrames(20)) {
    tickUpdate();
  }

  arduboy.pollButtons();

  if (arduboy.justPressed(A_BUTTON)) {
    currentScreen++;
    if (currentScreen >= 2) {
      currentScreen = 0; // 0 - server; 1 - office; 2 - settings
    }
  }
  if (currentScreen > 0) {
    if (arduboy.justReleased(LEFT_BUTTON)) {
      // toggle between office and settings
      if (currentScreen == 1) {
        currentScreen = 2; // 0 - server; 1 - office; 2 - settings
      } else {
        currentScreen = 1; // 0 - server; 1 - office; 2 - settings
      }
    }
  }

  arduboy.clear();
  if (introSequence >= SwitchToSmallHelpBoxID && finalSequence != 255) {
    if (currentScreen == 0){
      screenServer();
    }
    if (currentScreen == 1) {
      screenOffice();
    }
    if (currentScreen == 2) {
      screenSettings();
    }

    drawStatusBar();
  }

  compainionHelp();

  // Autosave logic
  if (autosave) {
    autosaveCounter++;
    if (autosaveCounter >= AUTOSAVE_INTERVAL) {
      saveGame();
      autosaveCounter = 0;
    }
  } else {
    autosaveCounter = 0;
  }

  // Save LED blink (fade in/out)
  if (saveLedTimer > 0) {
    uint8_t brightness;
    if (saveLedTimer > 10) {
      brightness = (20 - saveLedTimer) * 12; // fade in: 0 -> ~120
    } else {
      brightness = saveLedTimer * 12;         // fade out: ~120 -> 0
    }
    arduboy.setRGBled(0, brightness, 0);
    saveLedTimer--;
    if (saveLedTimer == 0) {
      arduboy.setRGBled(0, 0, 0);
    }
  }

  arduboy.display();
}
