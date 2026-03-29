#pragma once

uint8_t currentScreen = 1; // 0 - server; 1 - office

uint8_t currentFrame = 0;
uint8_t frameTimer = 0;
const uint8_t FRAME_COUNT = 20;
const uint8_t FRAME_DELAY = 8;  // game frames between animation steps (~7 fps at 60 fps)

bool music = true;

uint8_t aHoldFrames = 0;
const uint8_t A_HOLD_THRESHOLD = 120; // 3s at 60fps
uint8_t previousScreen = 0;



#define RackSize 4
#define MaxRacks 3
#define MaxServerLevel 20

uint8_t racksAmmount = 1;
//all racks
uint8_t racks[MaxRacks][RackSize] = {
  {0, 0, 0, 1},
  {0, 0, 0, 0},
  {0, 0, 0, 0}
};


// list of upgrades
struct Upgrade {
  String name;
  uint16_t cost;
  uint16_t bonus;
  uint16_t nextBonus;
  uint8_t have;
  uint8_t max;
};

#define MaxUpgrades 3

Upgrade upgrades[MaxUpgrades] = {
  {"Post on twiXer",  100,  0,  5,    0,  10},
  {"Buy ads",         200,  0,  10,   0,  100},
  {"Tv ad",           500,  0,  20,   0,  100}
};


