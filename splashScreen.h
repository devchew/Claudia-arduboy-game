#pragma once

#include <Arduboy2.h>
#include "Font3x5.h"
#include "image.h"
#include "State.h"
#include <ArduboyPlaytune.h>

bool blink = false;
bool splashScreenDone = false;
uint8_t blinkCounter = 0;


bool splashScreen() {
  if (splashScreenDone) {
    return true;
  }
  arduboy.pollButtons();

  if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON)) {
    splashScreenDone = true;
  }

  arduboy.clear();
  blinkCounter++;
  if (blink && blinkCounter >= 200) {
    blink = false;
    blinkCounter = 0;
  } else if (!blink && blinkCounter >= 30) {
    blink = true;
    blinkCounter = 0;
  }
  Sprites::drawOverwrite(76, 24, claudia, blink ? 0 : 1);
  arduboy.drawBitmap(20, 7, sprite_game_intro, 86, 32, WHITE);
  arduboy.drawBitmap(0, 53, sprite_devchew, 13, 11, WHITE);
  font3x5.setCursor(15, 57);
  font3x5.print(F("game"));

  arduboy.display();
  return false;
}
