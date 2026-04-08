
uint8_t listOffest = 0;
int8_t scrollAnimY = 0;
const int8_t SCROLL_SPEED = 4;

bool canPurchaseSelectedOfficeUpgrade() {
  return upgrades[listOffest].have < upgrades[listOffest].max;
}

void purchaseSelectedOfficeUpgrade() {
  //update cost, bonus, nextBonus, have

  //update const
  upgrades[listOffest].cost = getOfficeUpgradeCost(upgrades[listOffest]);

  //update bonus
  upgrades[listOffest].bonus = upgrades[listOffest].bonus + upgrades[listOffest].nextBonus;

  //update heve
  upgrades[listOffest].have++;

  //update nextBonus
  upgrades[listOffest].nextBonus *= 1.5;
}

uint32_t getOfficeUpgradePurchasePrice() {
  return upgrades[listOffest].cost;
}


void drawOfficeNavigation() {
  arduboy.fillRect(0, 56,128,8, BLACK);
  //left button
  Arduboy2Base::drawBitmap(45, 57, sprite_buttonsLeft, 7, 8, WHITE);
  font3x5.setCursor(55, 57);
  font3x5.print(F("Servers"));

  if (canPurchaseSelectedOfficeUpgrade()) {
    Arduboy2Base::drawBitmap(90, 57, sprite_buttonsRight, 7, 8, WHITE);
    font3x5.setCursor(100, 57);
    font3x5.print(F("Buy"));
  }

  font3x5.setCursor(2, 57);
  font3x5.print(F("<"));

  font3x5.setCursor(8, 57);
  font3x5.print(F("Settings"));

}

void drawUpgrade(int8_t x, int8_t y, uint8_t upgradeIndex) {
  arduboy.drawRoundRect(x, y, 120, 17, 1, WHITE);
  
  font3x5.setCursor(x+2, y+1);
  // font3x5.print(upgradesNames[upgradeIndex]);

  char buf[24];
  strcpy_P(buf, (const char*)pgm_read_ptr(&upgradesNames[upgradeIndex]));
  font3x5.print(buf);

  // font3x5.print(String(upgradeIndex));
  if (upgrades[upgradeIndex].have > 0) {
    font3x5.print(F(" x"));
    font3x5.print(upgrades[upgradeIndex].have);
    if (upgrades[upgradeIndex].have >= upgrades[upgradeIndex].max) {
      font3x5.print(F(" max"));
    }
  }

  // inbound
  arduboy.drawBitmap(x + 2, y+9, sprite_inboundSymbol, 13, 8, WHITE);
  font3x5.setCursor(x + 18, y+8);
  printValue(upgrades[upgradeIndex].bonus);

  // cost
  if (upgrades[upgradeIndex].max > upgrades[upgradeIndex].have) {
    arduboy.drawBitmap(x+51, y+8, sprite_currencySymbol, 5,8);
    font3x5.setCursor(x+58, y+8);
    printValue(upgrades[upgradeIndex].cost);

    arduboy.drawBitmap(x + 85, y+9, sprite_inboundSymbol, 13, 8, WHITE);
    font3x5.setCursor(x + 102, y+8);
    font3x5.print(F("+"));
    printValue(upgrades[upgradeIndex].nextBonus);
  }


}

void drawUpgades() {

  for(int i = -1; i <= 3; i++) {
    int8_t itemIndex = i + listOffest;
    if (itemIndex >= 0 && itemIndex < visibleUpgrades) {
      int8_t y = 12 + (i * 18) + scrollAnimY;
      if (y > -17 && y < 56) {
        drawUpgrade(5, y, itemIndex);
      }
    }
  }

  // clip top and bottom to hide partially scrolled items
  arduboy.fillRect(0, 10, 128, 2, BLACK);

  drawCursor(0, 18);
}


void screenOffice() {
  if (helpVisible) {
    return;
  }
  if (scrollAnimY == 0) {
    if (arduboy.justPressed(UP_BUTTON)) {
      if (listOffest == 0) {
        listOffest = visibleUpgrades - 1;
      } else {
        listOffest--;
      }
      scrollAnimY = -18;
    }

    if (arduboy.justPressed(DOWN_BUTTON)) {
      if (listOffest >= visibleUpgrades - 1) {
        listOffest = 0;
      } else {
        listOffest++;
      }
      scrollAnimY = 18;
    }
  }

  // animate scroll
  if (scrollAnimY > 0) {
    scrollAnimY -= SCROLL_SPEED;
    if (scrollAnimY < 0) scrollAnimY = 0;
  } else if (scrollAnimY < 0) {
    scrollAnimY += SCROLL_SPEED;
    if (scrollAnimY > 0) scrollAnimY = 0;
  }

  if (arduboy.justPressed(B_BUTTON)) {
    // buy or upgrade

    if (canPurchaseSelectedOfficeUpgrade() && buyIfPosible(getOfficeUpgradePurchasePrice())) {
      purchaseSelectedOfficeUpgrade();
      recalculateStats();
    }

  }

  drawUpgades();
  drawOfficeNavigation();
}