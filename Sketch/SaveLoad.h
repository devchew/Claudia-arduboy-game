#pragma once
#include <EEPROM.h>
#include "State.h"
#include "companion.h"

#define SAVE_MAGIC_0 0xAD
#define SAVE_MAGIC_1 0xBE

extern uint32_t money;
extern boolean helpProptsState[];
extern void recalculateStats();

bool hasSave() {
  uint16_t addr = EEPROM_STORAGE_SPACE_START;
  return EEPROM.read(addr) == SAVE_MAGIC_0 && EEPROM.read(addr + 1) == SAVE_MAGIC_1;
}

void saveGame() {
  uint16_t addr = EEPROM_STORAGE_SPACE_START;

  EEPROM.update(addr++, SAVE_MAGIC_0);
  EEPROM.update(addr++, SAVE_MAGIC_1);

  EEPROM.put(addr, money);
  addr += sizeof(money);

  EEPROM.update(addr++, introSequence);
  EEPROM.update(addr++, finalSequence);

  for (uint8_t i = 0; i < MaxHelpPrompts; i++) {
    EEPROM.update(addr++, helpProptsState[i] ? 1 : 0);
  }

  for (uint8_t r = 0; r < MaxRacks; r++) {
    for (uint8_t s = 0; s < RackSize; s++) {
      EEPROM.update(addr++, racks[r][s]);
    }
  }

  for (uint8_t u = 0; u < MaxUpgrades; u++) {
    EEPROM.update(addr++, upgrades[u].have);
  }

  EEPROM.update(addr++, autosave ? 1 : 0);
}

bool loadGame() {
  uint16_t addr = EEPROM_STORAGE_SPACE_START;

  if (EEPROM.read(addr++) != SAVE_MAGIC_0) return false;
  if (EEPROM.read(addr++) != SAVE_MAGIC_1) return false;

  EEPROM.get(addr, money);
  addr += sizeof(money);

  introSequence = EEPROM.read(addr++);
  finalSequence = EEPROM.read(addr++);

  for (uint8_t i = 0; i < MaxHelpPrompts; i++) {
    helpProptsState[i] = EEPROM.read(addr++) != 0;
  }

  for (uint8_t r = 0; r < MaxRacks; r++) {
    for (uint8_t s = 0; s < RackSize; s++) {
      racks[r][s] = EEPROM.read(addr++);
    }
  }

  for (uint8_t u = 0; u < MaxUpgrades; u++) {
    upgrades[u].have = EEPROM.read(addr++);
  }

  autosave = EEPROM.read(addr++) != 0;

  recalculateStats();
  return true;
}

void eraseSave() {
  uint16_t addr = EEPROM_STORAGE_SPACE_START;
  EEPROM.update(addr, 0xFF);
  EEPROM.update(addr + 1, 0xFF);
  autosave = false;
}
