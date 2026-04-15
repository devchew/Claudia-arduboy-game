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

ArduboyTones sound(arduboy.audio.enabled);

Arduboy2 arduboy;
Font3x5 font3x5 = Font3x5();


void setup() {
  arduboy.begin();
  arduboy.setFrameRate(60);
  recalculateStats();

  arduboy.audio.begin();
  // temporary disable music, because it is really annoying, and I want to focus on making the game fun first
  // music = arduboy.audio.enabled();
  music = false;

  if (music) {
    sound.tones(music_loop);
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

  if (finalSequence == 255) {
    screenTurnOffAnimation();
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

  // Your code here

  arduboy.display();
}
