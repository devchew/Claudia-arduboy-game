
uint8_t listOffest = 0;


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
  Arduboy2Base::drawBitmap(45, 57, buttonsLeft, 7, 8, WHITE);
  font3x5.setCursor(55, 57);
  font3x5.print(F("Servers"));

  if (canPurchaseSelectedOfficeUpgrade()) {
    Arduboy2Base::drawBitmap(90, 57, buttonsRight, 7, 8, WHITE);
    font3x5.setCursor(100, 57);
    font3x5.print(F("Buy"));
  }

}

void drawUpgrade(int8_t x, int8_t y, Upgrade upgrade) {
  arduboy.drawRoundRect(x, y, 120, 17, 1, WHITE);
  
  font3x5.setCursor(x+2, y+1);
  font3x5.print(upgrade.name);
  if (upgrade.have > 0) {
    font3x5.print(F(" x"));
    font3x5.print(upgrade.have);
    if (upgrade.have >= upgrade.max) {
      font3x5.print(F(" max"));
    }
  }

  // inbound
  arduboy.drawBitmap(x + 2, y+9, inboundSymbol, 13, 8, WHITE);
  font3x5.setCursor(x + 18, y+8);
  font3x5.print(parseValue(upgrade.bonus));

  // cost
  if (upgrade.max > upgrade.have) {
    font3x5.setCursor(x+68, y+8);
    arduboy.drawBitmap(x+61, y+8, currencySymbol, 5,8);
    font3x5.print(parseValue(upgrade.cost));

    arduboy.drawBitmap(x + 85, y+9, inboundSymbol, 13, 8, WHITE);
    font3x5.setCursor(x + 102, y+8);
    font3x5.print(F("+"));
    font3x5.print(parseValue(upgrade.nextBonus));
  }


}

void drawUpgades() {

  drawCursor(0, 18);

  for(int i = 0; i <= 2; i++) {
    if (i + listOffest >= MaxUpgrades) {
      continue;
    }
    drawUpgrade(5, 12 + (i*18), upgrades[i + listOffest]);
  }

}


void screenOffice() {
  if (helpVisible) {
    return;
  }
  if (arduboy.justPressed(UP_BUTTON)) {
    if (listOffest == 0) {
      listOffest = MaxUpgrades - 1;
    } else {
      listOffest--;
    }
  }

  if (arduboy.justPressed(DOWN_BUTTON)) {
    if (listOffest >= MaxUpgrades - 1) {
      listOffest = 0;
    } else {
      listOffest++;
    }
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