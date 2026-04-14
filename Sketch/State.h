#pragma once

uint8_t currentScreen = 1; // 0 - server; 1 - office; 2 - settings

bool music;

uint8_t introSequence = 0;
uint8_t finalSequence = 22;

#define MaxHelpPrompts 34
#define RackSize 4
#define MaxRacks 3
#define MaxServerLevel 20

//all racks
uint8_t racks[MaxRacks][RackSize] = {
  {0, 0, 0, 1},
  {0, 0, 0, 0},
  {0, 0, 0, 0}
};


// list of upgrades
struct Upgrade {
  uint32_t cost;
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


// 80
// 120
// 269
// 907
// 


Upgrade upgrades[MaxUpgrades] = {
/* cost    | bonus | nextB | have | max | */
  {80        ,  0  ,  10   ,  0   ,  5  }, // user messaging
  {1000      ,  0  ,  22   ,  0   ,  5  }, // push notifications
  {3500      ,  0  ,  45   ,  0   ,  2  }, // load balancing
  {12000     ,  0  ,  60   ,  0   ,  2  }, // request routing
  {40000     ,  0  ,  85   ,  0   ,  5  }, // async processing
  
// w tym momencie masz około 500k kasy więc ceny powinny być wysokie, ale bonusy też powinny być duże, żeby gracz czuł, że warto inwestować w te dodatki
  {200000    ,  0  ,  120  ,  0   ,  5  }, // ai core upgrade
  {400000    ,  0  ,  200  ,  0   ,  5  }, // ai context awareness
  {4000000   ,  0  ,  400  ,  0   ,  5  }, // ai smart responses
  {20000000  ,  0  ,  800  ,  0   ,  5  }, // predictive ai
  {800000000 ,  0  ,  1000 ,  0   ,  1  } // self evolving ai
};


