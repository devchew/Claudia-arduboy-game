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

#define OverloadWarningThreshold 105
#define OverloadPenaltyThreshold 190

uint8_t visibleUpgrades = 5;
uint8_t fulfilledRacksSlots = 0;


uint32_t getServerUpgradeCost(uint8_t level) {
  return serverPrice * pow(1.4, level);
}

uint32_t getServerUpgradeBonus(uint8_t level) {
  if (level == 0) {
    return 0;
  }
  uint32_t bonus = serverLevelCapacityScale;
  for (uint8_t i = 1; i < level; i++) {
    bonus = bonus * 1.3;
  }
  return bonus;
}

uint32_t getServerNextBonus(uint8_t level) {
  return getServerUpgradeBonus(level + 1) - getServerUpgradeBonus(level);
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

  fulfilledRacksSlots = 0;
  for(uint8_t r = 0; r < availableRacks; r++) {
    for(uint8_t s = 0; s < RackSize; s++) {
      totalCapacity += getServerUpgradeBonus(racks[r][s]);
      if (racks[r][s] >= MaxServerLevel) {
        fulfilledRacksSlots++;
      }
    }
  }


  loadPercent = inbound * 100 / max(totalCapacity, 1);

  // penalty for overload
  if (loadPercent > OverloadPenaltyThreshold) {
    inboundPenalty = true;
    inbound = inbound / 2;
    if (inbound > maxUsersPenaltyCap) {
      inbound = maxUsersPenaltyCap;
    }
  }

  // if async processing is purchased, show AI upgrades
  if (upgrades[4].have > 0 && visibleUpgrades <= 5) {
    visibleUpgrades = MaxUpgrades - 1; // unlock all but the last upgrade
    availableRacks = MaxRacks;
  }

  // if at least 3 ai upgrades maxed, unlock the last upgrade
  if (upgrades[5].have >= 1 && upgrades[6].have >= 1 && upgrades[7].have >= 1 && upgrades[8].have >= 1) {
    visibleUpgrades = MaxUpgrades; // unlock all upgrades
  }
}

void tickUpdate() {
  money += min(inbound, totalCapacity) * moneyPerUser;
}
