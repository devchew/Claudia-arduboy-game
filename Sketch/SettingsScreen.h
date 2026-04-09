#pragma once

#include "Font3x5.h"
#include "image.h"
#include "State.h"
#include "SaveLoad.h"
#include <ArduboyTones.h>

extern Arduboy2 arduboy;
extern Font3x5 font3x5;
extern ArduboyTones sound;
extern const uint16_t music_loop[] PROGMEM;

uint8_t settingsCursor = 0;
const uint8_t settingsCount = 3;

void drawSettingsNavigation() {
  arduboy.fillRect(0, 56, 128, 8, BLACK);

  //left button
  Arduboy2Base::drawBitmap(45, 57, sprite_buttonsLeft, 7, 8, WHITE);
  font3x5.setCursor(55, 57);
  font3x5.print(F("Servers"));

  Arduboy2Base::drawBitmap(90, 57, sprite_buttonsRight, 7, 8, WHITE);
  font3x5.setCursor(100, 57);
  font3x5.print(F("Toggle"));

  font3x5.setCursor(2, 57);
  font3x5.print(F("<"));

  font3x5.setCursor(8, 57);
  font3x5.print(F("Office"));

}

void drawSettingsItems() {
  drawCursor(0, (settingsCursor * 12) + 16);

  // Music toggle
  arduboy.drawRoundRect(5, 12, 120, 11, 1, WHITE);
  font3x5.setCursor(8, 14);
  font3x5.print(F("Music"));
  font3x5.setCursor(90, 14);
  font3x5.print(music ? F("ON") : F("OFF"));

  // Save game
  arduboy.drawRoundRect(5, 24, 120, 11, 1, WHITE);
  font3x5.setCursor(8, 26);
  font3x5.print(F("Save game"));

  // Load game
  arduboy.drawRoundRect(5, 36, 120, 11, 1, WHITE);
  font3x5.setCursor(8, 38);
  font3x5.print(F("Load game"));

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
        arduboy.audio.on();
        sound.tones(music_loop);
      } else {
        sound.noTone();
        arduboy.audio.off();
      }
      arduboy.audio.saveOnOff();
    }
    if (settingsCursor == 1) {
      saveGame();
    }
    if (settingsCursor == 2) {
      loadGame();
    }
  }

  drawSettingsItems();
  drawSettingsNavigation();
}
