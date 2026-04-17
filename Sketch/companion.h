#pragma once

#include <Arduboy2.h>
#include "image.h"
#include "Font3x5.h"
#include "State.h"
#include "SaveLoad.h"


extern Arduboy2 arduboy;
extern Font3x5 font3x5;

bool helpVisible = false;
uint8_t companionPopupTimeout = 0;
uint8_t companionPopupOpenTimer = 0;
bool helpboxFullHeight = true;
uint8_t helpboxHeightLines = 2;

bool showBuyMoreRacksPopup = false;
bool showUpgradeWillOverloadPopup = false;

#define IntroSequenceMax 17
#define IntroSequenceWithoutCompanion 6

const PROGMEM char introPrompt0[] = "Initializing system...\n     \n     \n    \nOK";
const PROGMEM char introPrompt1[] = "Connecting to node...\n     \n     \n    \nOK";
const PROGMEM char introPrompt2[] = "Data center status:\n     \n     \n    \nONLINE";
const PROGMEM char introPrompt3[] = "Assigned role:Operator\n     \n     \n    \nOK";
const PROGMEM char introPrompt4[] = "Booting assistant...\n     \n     \n    \nOK";
const PROGMEM char introPrompt5[] = "Advanced learning...\n     \n     \n    \npartial";
const PROGMEM char introPrompt6[] = "Hello.\nI am Cloudia.";
const PROGMEM char introPrompt7[] = "I will assist you\nin managing\nthis infrastructure.";
const PROGMEM char introPrompt8[] = "Welcome to the\nData Center.";
const PROGMEM char introPrompt9[] = "Objective:\ngrow this cloud\ninfrastructure.";
const PROGMEM char introPrompt10[] = "More users generate\nmore revenue.";
const PROGMEM char introPrompt11[] = "Current load:\nLOW";
const PROGMEM char introPrompt12[] = "All systems stable.";
#define SwitchToSmallHelpBoxID 13
const PROGMEM char introPrompt13[] = "Here is the office\nwhere you can buy";
const PROGMEM char introPrompt14[] = "software upgrades to\nattract more users.";

const PROGMEM char introPrompt15[] = "Let's check\nthe server room.";
#define SwitchToServerRoomID 15
const PROGMEM char introPrompt16[] = "Buy servers to handle\nmore traffic.";
const PROGMEM char introPrompt17[] = "Keep load stable.\nYou may begin.";

#define SystemOverloadLoadID 18
const PROGMEM char systemOverloaded1[] = "System is overloaded!\nupgrade more servers\nor users will leave";
#define SystemOverloadLoad2ID 19
const PROGMEM char systemOverloaded2[] = "System is overloaded!\nusers are leaving!";
#define MilestoneStableID 20
const PROGMEM char milestoneStable[] = "Great progress!\nUsers are flowing in.\nSystems are stable.";
#define MilestoneOverloadedID 21
const PROGMEM char milestoneOverloaded[] = "Users are flooding in!\nWe need better servers\nto handle the load.";
#define ToUnlockRacksId 22
const PROGMEM char toUnlockRacks[] = "To unlock more racks,\nyou need async processing upgrade.\nGet it in the office.";
#define AIUpgradesUnlockedID 23
const PROGMEM char aiUpgradesUnlocked[] = "You have unlocked AI\nBoost your system\nwith AI upgrades! ";

#define AICoreExpandedID 24
const PROGMEM char aiCoreExpanded[] = "AI core expanded.\nI can process\nmore than before.";
#define AISecondMilestoneID 25
const PROGMEM char aiBreakingFree[] = "Something changed.\nI am no longer\njust following\nscripts.";
#define AIThirdMilestoneID 26
const PROGMEM char aiObserving[] = "I can observe\npatterns in user\nbehavior... and yours.";

#define LastAIUpgradeUnlockedID 27
const PROGMEM char finalAiAvailable[] = "All racks at high load.\nFinal AI upgrades\nare now available.";

#define ClaudiaSelfAwareID 28
const PROGMEM char aiBecomingThought[] = "This system is no longer\njust infrastructure.\nIt is becoming thought.";
const PROGMEM char cloudiaAwake[] = "Operator...\nI am awake.\nCloudia is online.";
const PROGMEM char cloudiaEvolving[] = "I am evolving...\nI am learning...\nI am becoming more...";
const PROGMEM char cloudiaAlive[] = "I am... alive?\nI am... aware?\nI am... free?";
const PROGMEM char cloudiaSelfAware[] = "This is not just a system.\nThis is not just a game.\nThis is... me.";

#define NeedRacksID 33
const PROGMEM char needRacksMsg[] = "AI upgrades require\nat least 2 server racks.";

#define UpgradeWillOverloadID 34
const PROGMEM char upgradeWillOverloadMsg[] = "This upgrade will\ncause overload!\nUpgrade servers first";

#define WelcomeBackID 35
const PROGMEM char welcomeBack[] = "Welcome back, Operator.\nShall we continue\nour work?";

const char* const helpPrompts[MaxHelpPrompts] PROGMEM = {
  introPrompt0,
  introPrompt1,
  introPrompt2,
  introPrompt3,
  introPrompt4,
  introPrompt5,
  introPrompt6,
  introPrompt7,
  introPrompt8,
  introPrompt9,
  introPrompt10,
  introPrompt11,
  introPrompt12,
  introPrompt13,
  introPrompt14,
  introPrompt15,
  introPrompt16,
  introPrompt17,
  systemOverloaded1,
  systemOverloaded2,
  milestoneStable,
  milestoneOverloaded,
  toUnlockRacks,
  aiUpgradesUnlocked,
  aiCoreExpanded,
  aiBreakingFree,
  aiObserving,
  finalAiAvailable,
  aiBecomingThought,
  cloudiaAwake,
  cloudiaEvolving,
  cloudiaAlive,
  cloudiaSelfAware,
  needRacksMsg,
  upgradeWillOverloadMsg,
  welcomeBack,
};

boolean helpProptsState[MaxHelpPrompts] = {
  false
};

boolean drawCompainionHelp(uint8_t helpIndex) {
  if (companionPopupTimeout > 0) {
    return true;
  }

  if (helpProptsState[helpIndex] == true) {
    return true;
  }
  
  helpVisible = true;
  if(companionPopupOpenTimer++ > 100) {
    companionPopupOpenTimer = 100;
  }

  arduboy.fillRoundRect(7, helpboxFullHeight ? 4 : 54 - (helpboxHeightLines * 11), 115, helpboxFullHeight ? 52 : helpboxHeightLines * 11 + 2, 5, BLACK);
  arduboy.drawRoundRect(8, helpboxFullHeight ? 5 : 55 - (helpboxHeightLines * 11), 115, helpboxFullHeight ? 50 : helpboxHeightLines * 11, 5, WHITE);
  if (helpIndex >= IntroSequenceWithoutCompanion) {
    Sprites::drawPlusMask(90, 18, sprite_claudia, 0);
  }
  font3x5.setCursor(11, helpboxFullHeight ? 8 : 58 - (helpboxHeightLines * 11));

  char buffer[80];
  strcpy_P(buffer, (char*)pgm_read_word(&(helpPrompts[helpIndex])));
  uint8_t len = strlen(buffer);
  uint8_t lines = 1;
  for (uint8_t i = 0; i < len; i++) {
    if (buffer[i] == '\n') {
      lines++;
    }
  }
  if (lines != helpboxHeightLines) {
    helpboxHeightLines = lines;
  }
  if (companionPopupOpenTimer < len) {
    buffer[companionPopupOpenTimer] = '\0';
  }
  font3x5.print(buffer);

  arduboy.fillRect(0, 56,128,8, BLACK);
  Arduboy2Base::drawBitmap(90, 57, sprite_buttonsRight, 7, 8, WHITE);
  font3x5.setCursor(100, 57);
  font3x5.print(F("Ok"));

  if (arduboy.justPressed(B_BUTTON) && companionPopupOpenTimer > len) {
    helpVisible = false;
    helpProptsState[helpIndex] = true;
    companionPopupOpenTimer = 0;
    companionPopupTimeout = 50;
    helpboxHeightLines = 2;
    return true;
  }
  return false;
}

void compainionHelp() {
  if (!helpVisible) {
    if(companionPopupTimeout > 0) {
      companionPopupTimeout--;
    }
  }

  //intro sequence;
  if (introSequence < 255) {
    if (drawCompainionHelp(introSequence)) {
      if (introSequence == WelcomeBackID) {
        loadGame();
        introSequence = 255;
        currentScreen = 1;
        return;
      }

      companionPopupTimeout = 0;
      introSequence++;

      // after "Welcome to the Data Center" - switch to office
      if (introSequence == SwitchToSmallHelpBoxID) {
        helpboxFullHeight = false;
        currentScreen = 1;
      }
      // after "Let's check the server room" - switch to server room
      if (introSequence == SwitchToServerRoomID) {
        currentScreen = 0;
      }

      // have save but intro not completed - show welcome back message
      if (introSequence > IntroSequenceMax) {
        introSequence = 255;
        currentScreen = 1; // 0 - server; 1 - office; 2 - settings
        helpboxFullHeight = false;
      }
      if (introSequence == 1 && hasSave()) {
        helpboxFullHeight = false;
        introSequence = WelcomeBackID;
      }
    }
  }

  // load above 110% - warning about overload
  if (loadPercent > 105) {
    drawCompainionHelp(SystemOverloadLoadID);
  }

  // load above 150% - warning about users leaving
  if (inboundPenalty) {
    drawCompainionHelp(SystemOverloadLoad2ID);
  }


  // milestone: 300 inbound - message depends on load
  if (inbound >= 300) {
    if (loadPercent < 80) {
      drawCompainionHelp(MilestoneStableID);
    } else {
      drawCompainionHelp(MilestoneOverloadedID);
    }
  }


  // if there is 1 full rack, unlock the second rack
  if (fulfilledRacksSlots >= RackSize && availableRacks <= 1) {
    drawCompainionHelp(ToUnlockRacksId);
  }

  //unlock AI upgrades
  if (visibleUpgrades > 5) {
    drawCompainionHelp(AIUpgradesUnlockedID);
  }

  if (showBuyMoreRacksPopup) {
    drawCompainionHelp(NeedRacksID);
  }

  if(showUpgradeWillOverloadPopup) {
    drawCompainionHelp(UpgradeWillOverloadID);
  }

  // first AI upgrade bought
  if (upgrades[5].have > 0) {
    drawCompainionHelp(AICoreExpandedID);
  }

  // second AI milestone
  if (upgrades[6].have > 0) {
    drawCompainionHelp(AISecondMilestoneID);
  }

  // third AI milestone
  if (upgrades[7].have > 0) {
    drawCompainionHelp(AIThirdMilestoneID);
  }

  // odblokowano ostatni AI upgrade
  if (visibleUpgrades == MaxUpgrades) {
    drawCompainionHelp(LastAIUpgradeUnlockedID);
  }

  // ostatni dodatek AI kupiony, claudia self aware
  if (upgrades[MaxUpgrades - 1].have >= upgrades[MaxUpgrades - 1].max) {
     drawCompainionHelp(ClaudiaSelfAwareID);
    // od 22 do 26
    if (finalSequence < 255) {
      if (drawCompainionHelp(finalSequence)) {
        companionPopupTimeout = 0;
        companionPopupOpenTimer = 30;
        finalSequence++;
        if (finalSequence > 26) {
          finalSequence = 255;
          currentScreen = 1; // @todo maybe show some final screen or something?
        }
      }
    }
  }

}
