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

/**

todo:

- [] ekran ustawień bez przytrzymywania klawisza A, jako osobny screen w office < >
- [] zmiana powerupów na takie zwiaane z software, aby nawiązac od historii rozwoju AI
- [] dodanie komunikatow do akcji, gdzu rośnie ruch, gdy maleje, doanie intro z wprowadzeniem w historię


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
  
  if (++frameTimer >= FRAME_DELAY) {
    frameTimer = 0;
    if (++currentFrame >= FRAME_COUNT) currentFrame = 0;

  }

  if (!tune.playing() && music) {
    tune.playScore(music_loop);
  }

  // every x frames, update the game state
  if (currentFrame == 0 && introSequence == 255) {
    tickUpdate();
  }

  arduboy.pollButtons();

  if (!helpVisible) {
    if (currentScreen == 2) {
      if (arduboy.justPressed(A_BUTTON)) {
        currentScreen = previousScreen;
      }
    } else {
      if (arduboy.pressed(A_BUTTON)) {
        aHoldFrames++;
        if (aHoldFrames == A_HOLD_THRESHOLD) {
          previousScreen = currentScreen;
          currentScreen = 2;
        }
      } else {
        if (aHoldFrames > 0 && aHoldFrames < A_HOLD_THRESHOLD) {
          currentScreen = (currentScreen + 1) % 2;
        }
        aHoldFrames = 0;
      }
    }
  }

  arduboy.clear();
  if (introSequence == 255) {
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

  compainionHelp();

  // Your code here

  arduboy.display();
}
