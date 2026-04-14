#pragma once

#include <Arduboy2.h>
#include "image.h"
#include "Font3x5.h"
#include "State.h"
#include "commonUi.h"
#include "gameLogic.h"

extern Arduboy2 arduboy;
extern Font3x5 font3x5;
//cursor 
uint8_t cursorPosition = 3;
uint8_t visibleRack = 0;

// rack scroll animation
int8_t rackScrollX = 0;
uint8_t prevVisibleRack = 0;
bool prevRackEmpty = false;
const int8_t RACK_SCROLL_SPEED = 10;
const int8_t RACK_SCROLL_WIDTH = 80;

void drawServerSlot(uint8_t x, uint8_t y) {
  arduboy.drawBitmap(x, y, sprite_serverBlank, 28, 8, WHITE);
}


void drawServer(uint8_t x, uint8_t y, uint8_t level) {
  arduboy.drawRoundRect(x,y,28,8,1,WHITE);
  for (int i = 0; i < 5 && level > i * 4; i++) {
    uint8_t remaining = level - (i * 4);
    uint8_t fill = remaining > 4 ? 4 : remaining;
    arduboy.drawBitmap(x+2 + (i * 5), y+2, (const uint8_t*)pgm_read_ptr(&sprite_serverSlots[fill - 1]), 4, 8, WHITE);
  }
}


void drawRack(int8_t x, uint8_t y, uint8_t state[RackSize]) {

  if (currentRackEmpty) {
    Sprites::drawOverwrite(x, y, sprite_rack, 1);
    return;
  }
  Sprites::drawOverwrite(x, y, sprite_rack, 0);
  uint8_t fc = arduboy.frameCount >> 3; // slow down ~8x (every 8 frames)
  arduboy.drawPixel(28 + x, y + 1, ((fc >> 0) ^ (fc >> 2) ^ (fc >> 6)) & 1 ? WHITE : BLACK);
  arduboy.drawPixel(30 + x, y + 1, ((fc >> 1) ^ (fc >> 4) ^ (fc >> 6)) & 1 ? WHITE : BLACK);
  arduboy.drawPixel(32 + x, y + 1, ((fc >> 1) ^ (fc >> 3) ^ (fc >> 5)) & 1 ? WHITE : BLACK);
  arduboy.drawPixel(34 + x, y + 1, ((fc >> 2) ^ (fc >> 4) ^ (fc >> 7)) & 1 ? WHITE : BLACK);
  
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
  
  arduboy.drawLine(85, 11, 85, 26, WHITE);
  arduboy.drawLine(85, 26, 126, 26, WHITE);

  font3x5.setCursor(88, 11);
  font3x5.print(F("Capacity"));

  arduboy.drawBitmap(88, 19, sprite_inboundSymbol, 13, 8, WHITE);
  font3x5.setCursor(107, 18);
  printValue(totalCapacity);

}

bool canPurchaseSelectedServer() {
  if (currentRackEmpty) {
    return true;
  }
  return racks[visibleRack][cursorPosition] < MaxServerLevel;
}

void purchaseSelectedServer() {
  if (currentRackEmpty) {
    racks[visibleRack][3] = 1;
    currentRackEmpty = false;
    return;
  }
  racks[visibleRack][cursorPosition]++;
}

uint32_t getServerPurchasePrice() {
  if (currentRackEmpty) {
    return rackPrice;
  }

  if (racks[visibleRack][cursorPosition] > 0) {
    return getServerUpgradeCost(racks[visibleRack][cursorPosition]);
  }

  return serverPrice;
}

uint32_t getServerNextBonus() {
  return serverLevelCapacityScale;
}

void drawSelectedCost() {
  if (!canPurchaseSelectedServer()){
    return;
  }

  font3x5.setCursor(85, 27);
  if (racks[visibleRack][cursorPosition] > 0) {
    font3x5.print(F("Upgrade"));
  } else {
    font3x5.print(F("Buy"));
  }
  
  //price
  arduboy.drawBitmap(85, 37, sprite_currencySymbol, 5,8);
  font3x5.setCursor(93, 37);
  printValue(getServerPurchasePrice());

  // bonus
  arduboy.drawBitmap(85, 46, sprite_inboundSymbol, 13,8);
  font3x5.setCursor(101, 45);
  font3x5.print(F("+"));
  printValue(getServerNextBonus());
}

void drawServersNavigation() {
  //left button
  Arduboy2Base::drawBitmap(45, 57, sprite_buttonsLeft, 7, 8, WHITE);
  font3x5.setCursor(55, 57);
  font3x5.print(F("Office"));

  Arduboy2Base::drawBitmap(90, 57, sprite_buttonsRight, 7, 8, WHITE);
  font3x5.setCursor(100, 57);
  if (racks[visibleRack][cursorPosition] > 0) {
    font3x5.print(F("Upgrade"));
  } else {
    font3x5.print(F("Buy"));
  }

  if (availableRacks > 1) {
    font3x5.setCursor(8, 57);
    font3x5.print(F("<"));

    font3x5.setCursor(37, 57);
    font3x5.print(F(">"));

    font3x5.setCursor(19, 57);
    font3x5.print(visibleRack + 1);
    font3x5.print(F("/"));
    font3x5.print(availableRacks);
  }

}

void changeRack() {
  currentRackEmpty = true;
  for(int i = 0; i < RackSize; i++) {
    currentRackEmpty = racks[visibleRack][i] == 0 && currentRackEmpty == true;
  }
}

void screenServer() {
  if (helpboxFullHeight) {
    return;
  }
  if (!helpVisible) {
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

    if (availableRacks > 1 && rackScrollX == 0) {
      if (arduboy.justPressed(LEFT_BUTTON)) {
        prevVisibleRack = visibleRack;
        prevRackEmpty = currentRackEmpty;
        if (visibleRack < 1) {
          visibleRack = availableRacks - 1;
        } else {
          visibleRack--;
        }
        changeRack();
        rackScrollX = -RACK_SCROLL_WIDTH;
      }
      if (arduboy.justPressed(RIGHT_BUTTON)) {
        prevVisibleRack = visibleRack;
        prevRackEmpty = currentRackEmpty;
        if (visibleRack >= availableRacks - 1) {
          visibleRack = 0;
        } else {
          visibleRack++;
        }
        changeRack();
        rackScrollX = RACK_SCROLL_WIDTH;
      }
    }
  }

  // animate rack scroll
  if (rackScrollX > 0) {
    rackScrollX -= RACK_SCROLL_SPEED;
    if (rackScrollX < 0) rackScrollX = 0;
  } else if (rackScrollX < 0) {
    rackScrollX += RACK_SCROLL_SPEED;
    if (rackScrollX > 0) rackScrollX = 0;
  }

  if (arduboy.justPressed(B_BUTTON)) {
    // buy or upgrade
    if (canPurchaseSelectedServer() && buyIfPosible(getServerPurchasePrice())) {
      purchaseSelectedServer();
      recalculateStats();
    }
  }



  if (rackScrollX != 0) {
    // draw old rack sliding out
    bool savedEmpty = currentRackEmpty;
    currentRackEmpty = prevRackEmpty;
    int8_t oldRackX = 4 + rackScrollX + (rackScrollX < 0 ? RACK_SCROLL_WIDTH : -RACK_SCROLL_WIDTH);
    drawRack(oldRackX, 11, racks[prevVisibleRack]);
    currentRackEmpty = savedEmpty;

    // draw new rack sliding in
    drawRack(4 + rackScrollX, 11, racks[visibleRack]);
  } else {
    drawRack(4, 11, racks[visibleRack]);
    drawCursor(0, (cursorPosition * 9) + 16);
  }

  // background to prevent artifacts during rack scroll
  arduboy.fillRect(85, 11, 42, 46, BLACK);
  drawSelectedCost();
  drawServerStats();
  drawServersNavigation();
}