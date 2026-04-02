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

const String helpPrompts[13] = {
  /* 0  */ "Initializing system...",
  /* 1  */ "Connecting to node...",
  /* 2  */ "Data center status:\nONLINE",
  /* 3  */ "Assigned role:\nOperator",
  /* 4  */ "Booting assistant...",
  /* 5  */ "Learning...",
  /* 6  */ "Hello.\nI am Cloud'ia.",
  /* 7  */ "I will assist you\nin managing\nthis infrastructure.",
  /* 8  */ "Current load:\nLOW",
  /* 9  */ "All systems stable.",
  /* 10 */ "You may begin.",
  /* 11 */ "System is overloaded\nbuy more servers\nor users will leave",
  /* 12 */ "System is overloaded\nusers are leaving!"
};

boolean helpProptsState[13] = {
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
    Sprites::drawOverwrite(90, 18, claudia, 0); //@todo blink eye fliping the 0
    arduboy.drawLine(12, 52, min(companionPopupCloseTimeout, 90) + 20, 52, WHITE);
  }
  font3x5.setCursor(11, 8);
  font3x5.print(helpPrompts[helpIndex]);

  arduboy.fillRect(0, 56,128,8, BLACK);
  Arduboy2Base::drawBitmap(90, 57, buttonsRight, 7, 8, WHITE);
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

  if (loadPercent > 105) {
    drawCompainionHelp(11);
  }
  if (inboundPenalty) {
    drawCompainionHelp(12);
  }
}
