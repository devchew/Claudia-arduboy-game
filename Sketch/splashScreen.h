#pragma once

#include <Arduboy2.h>
#include "Font3x5.h"
#include "image.h"
#include "State.h"

bool blink = false;
bool splashScreenDone = false;
uint8_t blinkCounter = 0;
bool showSplashMenu = false;
uint8_t splashMenuCursor = 0;


void splashScreenMenu() {
  bool saveExists = hasSave();
  uint8_t menuItemCount = saveExists ? 3 : 2;

  if (arduboy.justPressed(UP_BUTTON)) {
    if (splashMenuCursor == 0) {
      splashMenuCursor = menuItemCount - 1;
    } else {
      splashMenuCursor--;
    }
  }

  if (arduboy.justPressed(DOWN_BUTTON)) {
    if (splashMenuCursor >= menuItemCount - 1) {
      splashMenuCursor = 0;
    } else {
      splashMenuCursor++;
    }
  }

  if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON)) {
    uint8_t action = splashMenuCursor;
    if (!saveExists) action++;

    if (action == 0) {
      splashScreenDone = true;
      return;
    } else if (action == 1) {
      eraseSave();
      splashScreenDone = true;
      return;
    } else if (action == 2) {
      music = !music;
      if (music) {
        arduboy.audio.on();
        sound.tones(music_loop);
      } else {
        sound.noTone();
        arduboy.audio.off();
      }
      arduboy.audio.saveOnOff();
    }
  }

  arduboy.clear();

  uint8_t y = 20;
  uint8_t itemIndex = 0;

  if (saveExists) {
    if (splashMenuCursor == itemIndex) drawCursor(15, y);
    arduboy.drawRoundRect(20, y - 1, 90, 9, 1, WHITE);
    font3x5.setCursor(23, y);
    font3x5.print(F("Continue"));
    y += 12;
    itemIndex++;
  }

  if (splashMenuCursor == itemIndex) drawCursor(15, y);
  arduboy.drawRoundRect(20, y - 1, 90, 9, 1, WHITE);
  font3x5.setCursor(23, y);
  font3x5.print(F("New Game"));
  y += 12;
  itemIndex++;

  if (splashMenuCursor == itemIndex) drawCursor(15, y);
  arduboy.drawRoundRect(20, y - 1, 90, 9, 1, WHITE);
  font3x5.setCursor(23, y);
  font3x5.print(F("Music"));
  font3x5.setCursor(80, y);
  font3x5.print(music ? F("ON") : F("OFF"));

  arduboy.display();
}

bool splashScreen() {
  if (splashScreenDone) {
    return true;
  }
  arduboy.pollButtons();

  if (showSplashMenu) {
    splashScreenMenu();
    return false;
  }

  if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON)) {
    showSplashMenu = true;
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
  Sprites::drawPlusMask(76, 24, sprite_claudia, 0);
  if (!blink) {
    Sprites::drawOverwrite(76+30, 24+26, sprite_eye, 0);
  }
  arduboy.drawBitmap(20, 7, sprite_game_intro, 86, 32, WHITE);
  arduboy.drawBitmap(0, 53, sprite_devchew, 13, 11, WHITE);
  font3x5.setCursor(15, 57);
  font3x5.print(F("game"));

  arduboy.display();
  return false;
}
