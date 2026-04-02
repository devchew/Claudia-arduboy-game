#pragma once

uint8_t currentScreen = 1; // 0 - server; 1 - office; 2 - settings

uint8_t currentFrame = 0;
uint8_t frameTimer = 0;
const uint8_t FRAME_COUNT = 20;
const uint8_t FRAME_DELAY = 8;  // game frames between animation steps (~7 fps at 60 fps)

bool music = true;

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
  uint16_t cost;
  uint16_t bonus;
  uint16_t nextBonus;
  uint8_t have;
  uint8_t max;
};

#define MaxUpgrades 10


const PROGMEM char upgrade0Name[] = "User messaging";
const PROGMEM char upgrade1Name[] = "Push notifications";
const PROGMEM char upgrade2Name[] = "Load balancing";
const PROGMEM char upgrade3Name[] = "Request routing";
const PROGMEM char upgrade4Name[] = "Async processing";
const PROGMEM char upgrade5Name[] = "AI core upgrade";
const PROGMEM char upgrade6Name[] = "AI Context awareness";
const PROGMEM char upgrade7Name[] = "AI Smart responses";
const PROGMEM char upgrade8Name[] = "Predictive AI";
const PROGMEM char upgrade9Name[] = "Self evolving AI";

const char* const upgradesNames[MaxUpgrades] PROGMEM = {
  upgrade0Name,
  upgrade1Name,
  upgrade2Name,
  upgrade3Name,
  upgrade4Name,
  upgrade5Name,
  upgrade6Name,
  upgrade7Name,
  upgrade8Name,
  upgrade9Name
};


Upgrade upgrades[MaxUpgrades] = {
/* cost | bonus | nextB | have | max | */
  {80   ,  0  ,   2   ,  0   ,  5  },
  {140  ,  0  ,   5   ,  0   ,  5  },
  {100  ,  0  ,   10  ,  0   ,  2  },
  {180  ,  0  ,   14  ,  0   ,  2  },
  {260  ,  0  ,   20  ,  0   ,  5  },
  {500  ,  0  ,   30  ,  0   ,  5  },
  {800  ,  0  ,   50  ,  0   ,  5  },
  {1000 ,  0  ,   60  ,  0   ,  5  },
  {5000 ,  0  ,   80  ,  0   ,  5  },
  {10000,  0  ,   100 ,  0   ,  1  }
};


