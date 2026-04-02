#pragma once
#include <Arduboy2.h>
#include "State.h"

extern Arduboy2 arduboy;

uint32_t money = 350;
uint32_t inbound = 0;
uint16_t loadPercent = 0;
uint32_t totalCapacity = 0;
bool inboundPenalty = false;

const uint16_t serverPrice = 1000;
const uint16_t rackPrice = 10000;
uint8_t availableRacks = 1;
bool currentRackEmpty = false;

#define serverLevelCapacityScale 15
#define serverCostFactor 2
#define moneyPerUser 2
#define startingInbound 1
#define maxUsersPenaltyCap 10

bool buyIfPosible(uint32_t price) {
  if (money >= price) {
    money -= price;
    return true;
  }
  return false;
}

void recalculateStats() {
  inboundPenalty = false;

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


  loadPercent = inbound * 100 / max(totalCapacity, 1);

  // penalty for overload
  if (loadPercent > 190) {
    inboundPenalty = true;
    inbound = inbound / 2;
    if (inbound > maxUsersPenaltyCap) {
      inbound = maxUsersPenaltyCap;
    }
  }
}

void tickUpdate() {
  money += min(inbound, totalCapacity) * moneyPerUser;
}

uint32_t getServerUpgradeCost(uint8_t level) {
  return serverPrice * pow(1.4, level);
}

uint32_t getOfficeUpgradeCost(Upgrade upgrade){
  return upgrade.cost * pow(1.5, upgrade.have);
}