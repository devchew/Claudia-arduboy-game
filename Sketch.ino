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
#include "screenTurnOffAnimation.h"

/**

todo:

- [x] ostatnia porcja komunikatw claudii, wyswietla sie tylko pierwszy i od razu pojawia sie ekran końćowey.
brakuje pozstałych komunikatów, a powinny sie pojawiać po kolei co x sekund, a po ostatnim pojawiać ekran końcowy.
- [ ] ostatni upgrade powinien kosztować 1mln, a nie 100k
- [ ] poprzednie dwa upgrade poiwnny być droższe, żeby gracz nie kupował ich od razu po odblokowaniu

*/

ArduboyTones sound(arduboy.audio.enabled);

Arduboy2 arduboy;
Font3x5 font3x5 = Font3x5();


void setup() {
  arduboy.begin();
  arduboy.setFrameRate(60);
  recalculateStats();

  arduboy.audio.begin();
  music = arduboy.audio.enabled();

  if (music) {
    sound.tones(music_loop);
  }
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

  // if (!sound.playing() && music) {
  //   sound.tones(music_loop);
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
