
uint8_t listOffest = 0;

void drawOfficeNavigation() {
  arduboy.fillRect(0, 56,128,8, BLACK);
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
  font3x5.print(F("Servers"));

}

void drawUpgrade(int8_t x, int8_t y, Upgrade upgrade) {
  arduboy.drawRoundRect(x, y, 120, 17, 1, WHITE);

  font3x5.setCursor(x+2, y+1);
  font3x5.print(upgrade.name);

  // cost
  arduboy.drawBitmap(x+71+5, y+8, currencySymbol, 5,8);
  font3x5.setCursor(x+78+5, y+8);
  font3x5.print(parseValue(upgrade.cost));

  // inbound
  arduboy.drawBitmap(x + 9+6, y+10, inboundSymbol, 13, 8, WHITE);
  Arduboy2Base::drawBitmap(x + 26+6, y+11, plusSign, 3, 8, WHITE);
  font3x5.setCursor(x + 30+6, y+9);
  font3x5.print(parseValue(upgrade.bonus));
}

void drawUpgades() {

  drawCursor(0, 18);

  for(int i = 0; i <= 2; i++) {
    if (i + listOffest >= visibleUpgrades) {
      continue;
    }
    drawUpgrade(5, 12 + (i*18), upgrades[i + listOffest]);
  }

}

void screenOffice() {
  if (arduboy.justPressed(UP_BUTTON)) {
    if (listOffest == 0) {
      listOffest = visibleUpgrades - 1;
    } else {
      listOffest--;
    }
  }

  if (arduboy.justPressed(DOWN_BUTTON)) {
    if (listOffest >= visibleUpgrades - 1) {
      listOffest = 0;
    } else {
      listOffest++;
    }
  }

  drawUpgades();
  drawOfficeNavigation();
}