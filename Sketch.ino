#include <Arduboy2.h>
#include "image.h"
#include "Font3x5.h"
#include "ServersScreen.h"
#include "State.h"
#include "commonUi.h"
#include "OfficeScreen.h"
#include "gameLogic.h"

Arduboy2 arduboy;
Font3x5 font3x5 = Font3x5();


void setup() {
  arduboy.begin();
  arduboy.setFrameRate(60);
}


void loop() {
  if (!arduboy.nextFrame()) return;
  
  if (++frameTimer >= FRAME_DELAY) {
    frameTimer = 0;
    if (++currentFrame >= FRAME_COUNT) currentFrame = 0;

  }

  // every x frames, update the game state
  if (currentFrame == 0) {
    tickUpdate();
  }

  arduboy.pollButtons();

  if (arduboy.justPressed(B_BUTTON)) {
    currentScreen = (currentScreen + 1) % 2;
  }

  arduboy.clear();
  drawStatusBar();

  if (currentScreen == 0){
    screenServer();
  }
  if (currentScreen == 1) {
    screenOffice();
  }

  // Your code here

  arduboy.display();
}
