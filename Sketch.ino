#include <Arduboy2.h>
#include "image.h"
#include "Font3x5.h"
#include "ServersScreen.h"
#include "State.h"
#include "commonUi.h"
#include "OfficeScreen.h"
#include "SettingsScreen.h"
#include "gameLogic.h"

#include <ArduboyPlaytune.h>
#include "intro.h"
#include "loop.h"
#include "companion.h"
#include "splashScreen.h"
#include "screenTurnOffAnimation.h"

/**

todo:

- [x] pierwsza seria upgrade nie pozwala na zapełnienie pierwszego racka, trzeba podbić bonusy
wszystkie bonusy dają max 406 userów, wymaxowany serwer to 1.2k
upgrade office powinna kończyć się na 150% capacity serwera userów, aby przekroczyć capacity serwera
- [x] komunikat: 300 U/m - claudia: informacja, ze dobrze idzie, widać napływ userów, i w zalenosci od loadu komuniakt o stabilności systemu, albo o potrzebie dokupienia serwerów
- [x] gdy wykupimy minimum łącznie 60lvl serwera  - komunikat od claudi ze mamy juz dużo serwerów i trzeba zainwestować w kolejna serwerownie
- [x] koszt nowego racka: 100K, max 3 racki
- [x] overload od 110% - komunikat o overloadzie i potrzebie dokupienia serwerów
- [x] overload od 150% - komunikat o krytycznym overloadzie i odpływie userów
- [x] po wykupieniu 2go serwera odblokowują się kolejne upgrade do nr 7, komunikat o nowych upgrade
- [x] claudia zaczyna byc self aware po pierwszych ai update
- [x] po wykupieniu 3ch serwerów i minimum 3/4 wypełnienia każdego odblokowuje się ostatnia seria upgrade
- [ ] po zakupieniu ostatniego upgrade claudia wyswietla ostatni komunikat i konic gry

*/

ArduboyPlaytune tune(arduboy.audio.enabled);

Arduboy2 arduboy;
Font3x5 font3x5 = Font3x5();


void setup() {
  arduboy.begin();
  arduboy.setFrameRate(60);
  recalculateStats();

  arduboy.audio.on();
  tune.initChannel(PIN_SPEAKER_1);
  tune.initChannel(PIN_SPEAKER_2);

  tune.playScore(music_intro);
}


void loop() {
  if (!arduboy.nextFrame()) return;
  
  if (!splashScreen()) {
    return;
  }


  if (++frameTimer >= FRAME_DELAY) {
    frameTimer = 0;
    if (++currentFrame >= FRAME_COUNT) currentFrame = 0;

  }

  // if (!tune.playing() && music) {
  //   tune.playScore(music_loop);
  // }

  // every x frames, update the game state
  if (currentFrame == 0 && introSequence == 255) {
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
  if (introSequence == 255 && finalSequence != 255) {
    drawStatusBar();

    if (currentScreen == 0){
      screenServer();
    }
    if (currentScreen == 1) {
      screenOffice();
    }
    if (currentScreen == 2) {
      screenSettings();
    }
  }

  if (finalSequence == 255) {
    screenTurnOffAnimation();
  }

  compainionHelp();

  // Your code here

  arduboy.display();
}
