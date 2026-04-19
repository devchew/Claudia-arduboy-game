#pragma once

#include "Font3x5.h"
#include "image.h"
#include "State.h"
#include "SaveLoad.h"
#include <ArduboyTones.h>

extern Arduboy2 arduboy;
extern Font3x5 font3x5;
extern ArduboyTones sound;
// extern const uint16_t music_loop[] PROGMEM;

uint8_t settingsCursor = 0;
const uint8_t settingsCount = 4 ;

//settingsOptions
#define MusicToggleID 0
#define AutosaveToggleID 1
#define SaveGameID 2
#define LoadGameID 3

const PROGMEM char   SettingsOptionMusic[] = "Music";
const PROGMEM char   SettingsOptionAutosave[] = "Autosave";
const PROGMEM char   SettingsOptionSaveGame[] = "Save game";
const PROGMEM char   SettingsOptionLoadGame[] = "Load game";

const char* const settingsOptions[settingsCount] PROGMEM = {
  SettingsOptionMusic,
  SettingsOptionAutosave,
  SettingsOptionSaveGame,
  SettingsOptionLoadGame,
};


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
  drawCursor(0, (settingsCursor * 9) + 12);

  for (uint8_t i = 0; i < settingsCount; i++) {
    int8_t y = (i * 9) + 10;
    arduboy.drawRoundRect(5, y, 120, 9, 1, WHITE);
    char buf[24];
    strcpy_P(buf, (const char*)pgm_read_ptr(&settingsOptions[i]));
    font3x5.setCursor(8, y+1);
    font3x5.print(buf);
    font3x5.setCursor(100, y+1);

    if (i == MusicToggleID) {
      font3x5.print(music ? F("On") : F("Off"));
    }
    if (i == AutosaveToggleID) {
      font3x5.print(autosave ? F("On") : F("Off"));
    }

  }

}

void screenSettings() {
  if (!helpVisible) {
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
      if (settingsCursor == MusicToggleID) {
        music = !music;
        if (music) {
          arduboy.audio.on();
          // sound.tones(music_loop);
        } else {
          sound.noTone();
          arduboy.audio.off();
        }
        arduboy.audio.saveOnOff();
      }
      if (settingsCursor == AutosaveToggleID) {
        autosave = !autosave;
        if (autosave) {
          autosaveCounter = 0;
          saveGame();
        }
      }
      if (settingsCursor == SaveGameID) {
        saveGame();
      }
      if (settingsCursor == LoadGameID) {
        loadGame();
      }
    }
  }

  drawSettingsItems();
  drawSettingsNavigation();
}
