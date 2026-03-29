#pragma once

uint8_t currentScreen = 1; // 0 - server; 1 - office

uint8_t currentFrame = 0;
uint8_t frameTimer = 0;
const uint8_t FRAME_COUNT = 20;
const uint8_t FRAME_DELAY = 8;  // game frames between animation steps (~7 fps at 60 fps)


//state;

uint32_t money = 1;
uint32_t inbound = 1;
uint8_t loadPercent = 70;

#define RackSize 4
#define MaxRacks 3
#define MaxServerLevel 20

uint8_t racksAmmount = 1;
uint8_t visibleRack = 0;
//all racks
uint8_t racks[MaxRacks][RackSize] = {
  {0, 0, 1, 5},
  {5, 3, 7, 15},
  {2, 4, 8, 18}
};
uint32_t totalCapacity = 0;


// list of upgrades
struct Upgrade {
  String name;
  uint16_t cost;
  uint16_t bonus;
};

#define MaxUpgrades 3
uint8_t visibleUpgrades = 3;

Upgrade upgrades[MaxUpgrades] = {
  {"Post on twiXer", 100, 5},
  {"Buy ads", 200, 10},
  {"Tv ad", 500, 20}
};


