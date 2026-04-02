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
  uint16_t cost;
  uint16_t bonus;
  uint16_t nextBonus;
  uint8_t have;
  uint8_t max;
};

#define MaxUpgrades 4

String upgradesNames[MaxUpgrades] = {
  "User messaging",            
  "Push notifications",        
  // "Load balancing",            
  // "Request routing",           
  // "Async processing",          
  // "AI core upgrade",           
  // "AI Context awareness",      
  // "AI Smart responses",        
  // "Predictive AI",             
  "Self evolving AI",          
};

// String upgradesNames[MaxUpgrades] = {"AA"};

Upgrade upgrades[MaxUpgrades] = {
/* cost | bonus | nextB | have | max | */
  {80   ,  0  ,   2   ,  0   ,  5  },
  {140  ,  0  ,   5   ,  0   ,  5  },
  // {100  ,  0  ,   10  ,  0   ,  2  },
  // {180  ,  0  ,   14  ,  0   ,  2  },
  // {260  ,  0  ,   20  ,  0   ,  5  },
  // {500  ,  0  ,   30  ,  0   ,  5  },
  // {800  ,  0  ,   50  ,  0   ,  5  },
  // {1000 ,  0  ,   60  ,  0   ,  5  },
  // {5000 ,  0  ,   80  ,  0   ,  5  },
  {10000,  0  ,   100 ,  0   ,  1  }
};


