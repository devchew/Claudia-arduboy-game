#pragma once

#include <Arduboy2.h>
#include "image.h"
#include "Font3x5.h"
#include "State.h"
#include "commonUi.h"

extern Arduboy2 arduboy;
extern Font3x5 font3x5;
//cursor 
uint8_t cursorPosition = 0;

void drawServerSlot(uint8_t x, uint8_t y) {
  arduboy.drawBitmap(x, y, serverBlank, 28, 8, WHITE);
}


void drawServer(uint8_t x, uint8_t y, uint8_t level) {
  arduboy.drawRoundRect(x,y,28,8,1,WHITE);
  for (int i = 0; i <= level / 5; i++) {
    arduboy.drawBitmap(x+2 + (i * 5), y+2, serverSlots[max(min(level - (i * 4),3),3)], 4, 8, WHITE); //@todo fix the slot number
  }
}


void drawRack(uint8_t x, uint8_t y, uint8_t state[RackSize]) {
  Sprites::drawOverwrite(x, y, rack, 0);
  arduboy.drawPixel(32 + x, y + 1, currentFrame % 10 ? BLACK : WHITE); //@todo: tweak this to blink more randomly
  arduboy.drawPixel(34 + x, y + 1, currentFrame % 12 ? BLACK : WHITE); //@todo: tweak this to blink more randomly
  
  for(int i = 0; i < RackSize; i++) {
    font3x5.setCursor(40 + x, 4 + y + (i * 9));
    if (state[i] == 0) {
      drawServerSlot( 6 + x, 4 + y + (i * 9));
      font3x5.print(F("Buy"));
    } else {
      drawServer(     6 + x, 4 + y + (i * 9), state[i]);
      
      font3x5.print(F("LVL "));
      font3x5.print(String(state[i]));
    }
  }
}

void drawServerStats() {
  
  arduboy.drawLine(85, 11, 85, 50, WHITE);

  font3x5.setCursor(90, 12);
  font3x5.print(F("Capacity"));

  arduboy.drawBitmap(90, 23, inboundSymbol, 13, 8, WHITE);
  font3x5.setCursor(107, 22);
  font3x5.print(F("1M"));

}

void drawServersNavigation() {
  //left button
  Arduboy2Base::drawBitmap(45, 57, buttonsLeft, 7, 8, WHITE);
  font3x5.setCursor(55, 57);
  if (racks[visibleRack][cursorPosition] > 0) {
    font3x5.print(F("Upgrade"));
  } else {
    font3x5.print(F("Buy"));
  }

  Arduboy2Base::drawBitmap(90, 57, buttonsRight, 7, 8, WHITE);
  font3x5.setCursor(100, 57);
  font3x5.print(F("Office"));

}

void screenServer() {
  if (arduboy.justPressed(UP_BUTTON)) {
    if (cursorPosition == 0) {
      cursorPosition = RackSize - 1;
    } else {
      cursorPosition--;
    }
  }

  if (arduboy.justPressed(DOWN_BUTTON)) {
    if (cursorPosition >= RackSize - 1) {
      cursorPosition = 0;
    } else {
      cursorPosition++;
    }
  }


  drawRack(4, 11, racks[visibleRack]);
  drawCursor(0, (cursorPosition * 9) + 16);

  drawServerStats();
  drawServersNavigation();
}