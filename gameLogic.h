#pragma once
#include <Arduboy2.h>
#include "State.h"

extern Arduboy2 arduboy;


uint32_t money = 0;
uint32_t inbound = 0;
uint8_t loadPercent = 0;
uint32_t totalCapacity = 0;

const uint16_t serverPrice = 5000;

#define serverLevelCapacityScale 10
#define serverCostFactor 2
#define moneyPerUser 1
#define startingInbound 1

bool buyIfPosible(uint32_t price) {
  if (money >= price) {
    money -= price;
    return true;
  }
  return false;
}

void recalculateStats() {
  // inbound based on office upgrades
  inbound = startingInbound;

  for(uint8_t u = 0; u < MaxUpgrades; u++) {
    inbound += upgrades[u].bonus;
  }

  // total capacity based on server stats

  totalCapacity = 0;

  for(uint8_t r = 0; r < racksAmmount; r++) {
    for(uint8_t s = 0; s < RackSize; s++) {
      totalCapacity += racks[r][s] * serverLevelCapacityScale;
    }
  }


  loadPercent = min(inbound * 100 / max(totalCapacity, 1),100);
}

void tickUpdate() {
  money += min(inbound, totalCapacity) * moneyPerUser;
}

uint32_t getServerUpgradeCost(uint8_t level) {
  return level > 0 ? pow(serverCostFactor, level - 1) : 0;
}

uint32_t getOfficeUpgradeCost(Upgrade upgrade){
  return upgrade.cost + upgrade.nextBonus * upgrade.have;
}