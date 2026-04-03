#pragma once

#include <Arduboy2.h>
#include "image.h"
#include "Font3x5.h"
#include "State.h"


extern Arduboy2 arduboy;
extern Font3x5 font3x5;

bool helpVisible = false;
uint8_t companionPopupTimeout = 0;
uint8_t companionPopupCloseTimeout = 0;

uint8_t introSequence = 0;

const PROGMEM char helpPrompt0[] = "Initializing system...";
const PROGMEM char helpPrompt1[] = "Connecting to node...";
const PROGMEM char helpPrompt2[] = "Data center status:\nONLINE";
const PROGMEM char helpPrompt3[] = "Assigned role:\nOperator";
const PROGMEM char helpPrompt4[] = "Booting assistant...";
const PROGMEM char helpPrompt5[] = "Learning...";
const PROGMEM char helpPrompt6[] = "Hello.\nI am Cloud'ia.";
const PROGMEM char helpPrompt7[] = "I will assist you\nin managing\nthis infrastructure.";
const PROGMEM char helpPrompt8[] = "Current load:\nLOW";
const PROGMEM char helpPrompt9[] = "All systems stable.";
const PROGMEM char helpPrompt10[] = "You may begin.";
const PROGMEM char helpPrompt11[] = "System is overloaded\nbuy more servers\nor users will leave";
const PROGMEM char helpPrompt12[] = "System is overloaded\nusers are leaving!";
const PROGMEM char helpPrompt13[] = "Great progress!\nUsers are flowing in.\nSystems are stable.";
const PROGMEM char helpPrompt14[] = "Users are flooding in!\nWe need more servers\nto handle the load.";

const char* const helpPrompts[15] PROGMEM = {
  helpPrompt0,
  helpPrompt1,
  helpPrompt2,
  helpPrompt3,
  helpPrompt4,
  helpPrompt5,
  helpPrompt6,
  helpPrompt7,
  helpPrompt8,
  helpPrompt9,
  helpPrompt10,
  helpPrompt11,
  helpPrompt12,
  helpPrompt13,
  helpPrompt14
};

boolean helpProptsState[15] = {
  false
};

boolean drawCompainionHelp(uint8_t helpIndex) {
  if (companionPopupTimeout > 0) {
    return true;
  }

  if (helpProptsState[helpIndex] == true) {
    return true;
  }
  
  helpVisible = true;
  if(companionPopupCloseTimeout++ > 100) {
    companionPopupCloseTimeout = 100;
  }

  arduboy.fillRoundRect(8, 6, 115, 50, 5, BLACK);
  arduboy.drawRoundRect(8, 5, 115, 50, 5, WHITE);
  if (helpIndex >= 6) {
    Sprites::drawOverwrite(90, 18, sprite_claudia, 0); //@todo blink eye fliping the 0
    arduboy.drawLine(12, 52, min(companionPopupCloseTimeout, 90) + 20, 52, WHITE);
  }
  font3x5.setCursor(11, 8);

  char buffer[64];
  strcpy_P(buffer, (char*)pgm_read_word(&(helpPrompts[helpIndex])));
  font3x5.print(buffer);

  arduboy.fillRect(0, 56,128,8, BLACK);
  Arduboy2Base::drawBitmap(90, 57, sprite_buttonsRight, 7, 8, WHITE);
  font3x5.setCursor(100, 57);
  font3x5.print(F("Ok"));

  if (arduboy.justPressed(B_BUTTON) && companionPopupCloseTimeout > 50) {
    helpVisible = false;
    helpProptsState[helpIndex] = true;
    companionPopupCloseTimeout = 0;
    companionPopupTimeout = 150;
    return true;
  }
  return false;
}

void compainionHelp() {
  if (!helpVisible) {
    if(companionPopupTimeout > 0) {
      companionPopupTimeout--;
    }
  }

  //intro sequence;
  if (introSequence < 255) {
    if (drawCompainionHelp(introSequence)) {
      companionPopupTimeout = 0;
      companionPopupCloseTimeout = 30;
      introSequence++;
      if (introSequence > 10) {
        introSequence = 255;
        currentScreen = 1; // 0 - server; 1 - office; 2 - settings
      }
    }
  }

  // milestone: 300 inbound - message depends on load
  if (inbound >= 300 && !helpProptsState[13] && !helpProptsState[14]) {
    if (loadPercent < 80) {
      drawCompainionHelp(13);
    } else {
      drawCompainionHelp(14);
    }
  }

  if (loadPercent > 105) {
    drawCompainionHelp(11);
  }
  if (inboundPenalty) {
    drawCompainionHelp(12);
  }
}
