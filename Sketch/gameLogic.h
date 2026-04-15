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
const uint32_t rackPrice = 100000;
uint8_t availableRacks = 1;
bool currentRackEmpty = false;

#define serverLevelCapacityScale 19
#define serverCostFactor 2
#define moneyPerUser 1
#define startingInbound 1
#define maxUsersPenaltyCap 10

uint8_t visibleUpgrades = 5;
uint8_t filledRacksSlots = 0;


uint32_t getServerUpgradeCost(uint8_t level) {
  return serverPrice * pow(1.4, level);
}



uint32_t getOfficeUpgradeCost(Upgrade upgrade){
  return upgrade.startingPrice * pow(1.5, upgrade.have);
}

uint32_t getNextOfficeUpgradeCost(Upgrade upgrade){
  return getOfficeUpgradeCost(Upgrade{upgrade.startingPrice, upgrade.startingBonus, upgrade.have + 1, upgrade.max});
}

uint32_t getOfficeUpgradeBonus(Upgrade upgrade) {
  if (upgrade.have == 0) {
    return 0;
  }
  uint32_t bonus = upgrade.startingBonus;
  for (uint8_t i = 0; i < upgrade.have; i++) {
    bonus += bonus * 0.5;
  }
  return bonus;
}

uint32_t getNextOfficeUpgradeBonus(Upgrade upgrade) {
  return getOfficeUpgradeBonus(Upgrade{upgrade.startingPrice, upgrade.startingBonus, upgrade.have + 1, upgrade.max});
}

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
    inbound += getOfficeUpgradeBonus(upgrades[u]);
  }

  // total capacity based on server stats

  totalCapacity = 0;

  filledRacksSlots = 0;
  for(uint8_t r = 0; r < availableRacks; r++) {
    for(uint8_t s = 0; s < RackSize; s++) {
      totalCapacity += racks[r][s] * serverLevelCapacityScale;
      if (racks[r][s] > 0) {
        filledRacksSlots++;
      }
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

  // if there is at least total level 50 of servers, unlock all racks
  // level 50 * serverLevelCapacityScale 15 = 750 total capacity
  if (totalCapacity >= 750 && availableRacks <= 1) {
    availableRacks = MaxRacks;
  }

  // if there is at least morse than 1 rack fully filled, unlock all but the last upgrade
  if (filledRacksSlots > RackSize && visibleUpgrades <= 5) {
    visibleUpgrades = MaxUpgrades - 1; // unlock all but the last upgrade
  }

  // if at least 3 ai upgrades maxed, unlock the last upgrade
  if (upgrades[5].have >= upgrades[5].max && upgrades[6].have >= upgrades[6].max && upgrades[7].have >= upgrades[7].max && upgrades[8].have >= upgrades[8].max) {
    visibleUpgrades = MaxUpgrades; // unlock all upgrades
  }
}

void tickUpdate() {
  money += min(inbound, totalCapacity) * moneyPerUser;
}
