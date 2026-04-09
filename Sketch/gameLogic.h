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

// Initial nextBonus values matching the upgrades[] initializer in State.h.
// Used by recalculateStats() to derive bonus/nextBonus from have, so that
// inbound is always correct after a load even if the save had stale/missing bonus bytes.
const uint16_t upgradeInitNextBonus[MaxUpgrades] = {10, 22, 45, 60, 85, 120, 200, 400, 800, 1000};

uint8_t visibleUpgrades = 5;
uint8_t filledRacksSlots = 0;

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
  // Recompute bonus and nextBonus from have + initial values so that inbound
  // is always correct after a load (avoids relying on saved bonus bytes).
  inbound = startingInbound;

  for(uint8_t u = 0; u < MaxUpgrades; u++) {
    uint16_t nb = upgradeInitNextBonus[u];
    uint16_t b = 0;
    for (uint8_t i = 0; i < upgrades[u].have; i++) {
      b += nb;
      nb = (uint16_t)(nb * 1.5f);
    }
    upgrades[u].bonus = b;
    upgrades[u].nextBonus = nb;
    inbound += b;
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

uint32_t getServerUpgradeCost(uint8_t level) {
  return serverPrice * pow(1.4, level);
}

uint32_t getOfficeUpgradeCost(Upgrade upgrade){
  return upgrade.cost * pow(1.5, upgrade.have);
}