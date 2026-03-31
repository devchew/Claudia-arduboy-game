#pragma once

#include <Arduboy2.h>
#include "image.h"
#include "Font3x5.h"
#include "State.h"


extern Arduboy2 arduboy;
extern Font3x5 font3x5;

bool helpVisible = false;
uint8_t companionPopupTimeout = 0;

String helpPrompts[] = {
  "System is overloaded\nbuy more servers\nor users will leave"
};

boolean helpProptsState[] = {
  false
};

void drawCompainionHelp(uint8_t helpIndex) {
  if (helpProptsState[helpIndex]) {
    return;
  }
  
  helpVisible = true;
  companionPopupTimeout++;
  if (companionPopupTimeout > 100) {
    companionPopupTimeout = 100;
  }

  arduboy.fillRoundRect(8, 6, 115, 50, 5, BLACK);
  arduboy.drawRoundRect(8, 5, 115, 50, 5, WHITE);
  Sprites::drawOverwrite(90, 18, claudia, 1);
  font3x5.setCursor(11, 8);
  font3x5.print(helpPrompts[helpIndex]);

  arduboy.fillRect(0, 56,128,8, BLACK);
  Arduboy2Base::drawBitmap(90, 57, buttonsRight, 7, 8, WHITE);
  font3x5.setCursor(100, 57);
  font3x5.print(F("Close"));
  arduboy.drawLine(12, 52, companionPopupTimeout + 20, 52, WHITE);

  if (arduboy.justPressed(B_BUTTON) && companionPopupTimeout > 50) {
    helpVisible = false;
    helpProptsState[helpIndex] = true;
    companionPopupTimeout = 0;
  }
}

void compainionHelp() {
  if (loadPercent > 105) {
    drawCompainionHelp(0);
  }
}
