#pragma once

#include <Arduboy2.h>
#include "image.h"
#include "Font3x5.h"
#include "State.h"
#include "gameLogic.h"

#include "companion.h"

extern Arduboy2 arduboy;
extern Font3x5 font3x5;

void printValue(uint32_t value) {
  if (value < 1000) {
    font3x5.print(value);
  } else if (value < 1000000) {
    font3x5.print(String(value / 1000.0, 1) + "K");
  } else if (value < 1000000000) {
    font3x5.print(String(value / 1000000.0, 1) + "M");
  } else {
    font3x5.print(String(value / 1000000000.0, 1) + "B");
  }
}

void drawCursor(uint8_t x, uint8_t y) {
  arduboy.drawBitmap(x, y, sprite_cursor, 5, 8, WHITE);
}

void drawStatusBar() {
  arduboy.drawLine(0, 9, 128, 9, WHITE);

  // money
  arduboy.drawBitmap(1,1, sprite_currencySymbol, 5,8);
  font3x5.setCursor(8, 1);
  printValue(money);

  // inbound
  arduboy.drawBitmap(35, 2, sprite_inboundSymbol, 13, 8, WHITE);
  font3x5.setCursor(50, 1);
  printValue(inbound);
  if (inboundPenalty) {
    font3x5.print(F("!"));
  }

  //load
  arduboy.drawBitmap(70,1, sprite_loadSymbol, 5, 8, WHITE);
  arduboy.drawRect(77, 1, 50, 7, WHITE);
  arduboy.drawBitmap(78, 2, sprite_loadBarFilled, min(loadPercent/2,48) , 8);
  //linfill;
  if (loadPercent < 100) {
    arduboy.drawLine(77 + loadPercent/2, 1, 77 + loadPercent/2, 7, WHITE);
  }

  if (loadPercent > 105) {
    arduboy.fillRoundRect(85, 2, 33, 5, 1, BLACK);
    font3x5.setCursor(86, 1);
    font3x5.print(F("OVERLOAD"));
  }
  
}

