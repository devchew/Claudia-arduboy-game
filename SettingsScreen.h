#pragma once

#include <Arduboy2.h>
#include "Font3x5.h"
#include "image.h"
#include "State.h"
#include <ArduboyPlaytune.h>

extern Arduboy2 arduboy;
extern Font3x5 font3x5;
extern ArduboyPlaytune tune;
extern const byte music_intro[] PROGMEM;

uint8_t settingsCursor = 0;
const uint8_t settingsCount = 2;

void drawSettingsNavigation() {
  arduboy.fillRect(0, 56, 128, 8, BLACK);

  Arduboy2Base::drawBitmap(45, 57, buttonsLeft, 7, 8, WHITE);
  font3x5.setCursor(55, 57);
  font3x5.print(F("Back"));

  Arduboy2Base::drawBitmap(90, 57, buttonsRight, 7, 8, WHITE);
  font3x5.setCursor(100, 57);
  font3x5.print(F("Toggle"));
}

void drawSettingsItems() {
  drawCursor(0, (settingsCursor * 12) + 16);

  // Music toggle
  arduboy.drawRoundRect(5, 12, 120, 11, 1, WHITE);
  font3x5.setCursor(8, 14);
  font3x5.print(F("Music"));
  font3x5.setCursor(90, 14);
  font3x5.print(music ? F("ON") : F("OFF"));

}

void screenSettings() {
  if (arduboy.justPressed(UP_BUTTON)) {
    if (settingsCursor == 0) {
      settingsCursor = settingsCount - 1;
    } else {
      settingsCursor--;
    }
  }

  if (arduboy.justPressed(DOWN_BUTTON)) {
    if (settingsCursor >= settingsCount - 1) {
      settingsCursor = 0;
    } else {
      settingsCursor++;
    }
  }

  if (arduboy.justPressed(B_BUTTON)) {
    if (settingsCursor == 0) {
      music = !music;
      if (music) {
        tune.playScore(music_intro);
      } else {
        tune.stopScore();
      }
    }
  }

  drawSettingsItems();
  drawSettingsNavigation();
}
