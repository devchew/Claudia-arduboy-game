#pragma once

#include <Arduboy2.h>
#include "image.h"
#include "Font3x5.h"
#include "State.h"
#include "gameLogic.h"


extern Arduboy2 arduboy;
extern Font3x5 font3x5;

String parseValue(uint32_t value) {
  if (value < 1000) {
    return String(value);
  } else if (value < 1000000) {
    return String(value / 1000.0, 1) + "K";
  } else if (value < 1000000000) {
    return String(value / 1000000.0, 1) + "M";
  } else {
    return String(value / 1000000000.0, 1) + "B";
  }
}

void drawCursor(uint8_t x, uint8_t y) {
  arduboy.drawBitmap(x, y, cursor, 5, 8, WHITE);
}

void drawStatusBar() {
  arduboy.drawLine(0, 9, 128, 9, WHITE);

  // money
  arduboy.drawBitmap(3,1, currencySymbol, 5,8);
  font3x5.setCursor(10, 1);
  font3x5.print(parseValue(money));

  // inbound
  arduboy.drawBitmap(37, 2, inboundSymbol, 13, 8, WHITE);
  font3x5.setCursor(52, 1);
  font3x5.print(parseValue(inbound));

  //load
  arduboy.drawBitmap(70,1, loadSymbol, 5, 8, WHITE);
  arduboy.drawRect(77, 1, 50, 7, WHITE);
  arduboy.drawBitmap(78, 2, loadBarFilled, min(loadPercent/2,48) , 8);
  //linfill;
  if (loadPercent < 100) {
    arduboy.drawLine(77 + loadPercent/2, 1, 77 + loadPercent/2, 7, WHITE);
  }
  
}

