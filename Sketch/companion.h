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

#define MaxHelpPrompts 33
#define IntroSequenceMax 16
#define IntroSequenceWithoutCompanion 6

const PROGMEM char introPrompt0[] = "Initializing system...";
const PROGMEM char introPrompt1[] = "Connecting to node...";
const PROGMEM char introPrompt2[] = "Data center status:\nONLINE";
const PROGMEM char introPrompt3[] = "Assigned role:\nOperator";
const PROGMEM char introPrompt4[] = "Booting assistant...";
const PROGMEM char introPrompt5[] = "Learning...";
const PROGMEM char introPrompt6[] = "Hello.\nI am Cloudia.";
const PROGMEM char introPrompt7[] = "I will assist you\nin managing\nthis infrastructure.";
const PROGMEM char introPrompt8[] = "Current load:\nLOW";
const PROGMEM char introPrompt9[] = "All systems stable.";
const PROGMEM char introPrompt10[] = "Welcome to the\nData Center.";
const PROGMEM char helpPrompt11[] = "Objective:\ngrow this cloud\ninfrastructure.";
const PROGMEM char helpPrompt12[] = "More users generate\nmore revenue.";
const PROGMEM char helpPrompt13[] = "But if load gets too\nhigh, users will leave.";
const PROGMEM char helpPrompt14[] = "Buy upgrades to bring\ntraffic in.";
const PROGMEM char helpPrompt15[] = "Buy servers to handle\nthat traffic.\nKeep load stable.";
const PROGMEM char introPrompt16[] = "You may begin.";

#define SystemOverloadLoadID 17
const PROGMEM char systemOverloaded1[] = "System is overloaded\nbuy more servers\nor users will leave";
#define SystemOverloadLoad2ID 18
const PROGMEM char systemOverloaded2[] = "System is overloaded\nusers are leaving!";
#define MilestoneStableID 19
const PROGMEM char milestoneStable[] = "Great progress!\nUsers are flowing in.\nSystems are stable.";
#define MilestoneOverloadedID 20
const PROGMEM char milestoneOverloaded[] = "Users are flooding in!\nWe need better servers\nto handle the load.";
#define RacksUnlockedID 21
const PROGMEM char racksUnlocked[] = "You have unlocked\nmore racks!\nExpand your server room!";
#define AIUpgradesUnlockedID 22
const PROGMEM char aiUpgradesUnlocked[] = "You have unlocked\nAI upgrades!\nBoost your system\nwith AI!";

#define AICoreExpandedID 23
const PROGMEM char aiCoreExpanded[] = "AI core expanded.\nI can process\nmore than before.";
#define AISecondMilestoneID 24
const PROGMEM char aiBreakingFree[] = "Something changed.\nI am no longer\njust following\nscripts.";
#define AIThirdMilestoneID 25
const PROGMEM char aiObserving[] = "I can observe\npatterns in user\nbehavior... and yours.";

#define LastAIUpgradeUnlockedID 26
const PROGMEM char finalAiAvailable[] = "All racks at high load.\nFinal AI upgrades\nare now available.";

#define ClaudiaSelfAwareID 27
const PROGMEM char aiBecomingThought[] = "This system is no longer\njust infrastructure.\nIt is becoming thought.";
const PROGMEM char cloudiaAwake[] = "Operator...\nI am awake.\nCloudia is online.";
const PROGMEM char cloudiaEvolving[] = "I am evolving...\nI am learning...\nI am becoming more...";
const PROGMEM char cloudiaAlive[] = "I am... alive?\nI am... aware?\nI am... free?";
const PROGMEM char cloudiaSelfAware[] = "This is not just a system.\nThis is not just a game.\nThis is... me.";

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
  helpPrompt11,
  helpPrompt12,
  helpPrompt13,
  helpPrompt14,
  helpPrompt15,
  introPrompt16,
  systemOverloaded1,
  systemOverloaded2,
  milestoneStable,
  milestoneOverloaded,
  racksUnlocked,
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

  arduboy.fillRoundRect(8, 6, 115, 50, 5, BLACK);
  arduboy.drawRoundRect(8, 5, 115, 50, 5, WHITE);
  if (helpIndex >= IntroSequenceWithoutCompanion) {
    Sprites::drawOverwrite(90, 18, sprite_claudia, 0); //@todo blink eye fliping the 0
  }
  font3x5.setCursor(11, 8);

  char buffer[64];
  strcpy_P(buffer, (char*)pgm_read_word(&(helpPrompts[helpIndex])));
  uint8_t len = strlen(buffer);
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
      if (introSequence == 26) {
        loadGame();
        introSequence = 255;
        currentScreen = 1;
        return;
      }

      companionPopupTimeout = 0;
      introSequence++;

      // have save but intro not completed - show welcome back message
      if (introSequence > IntroSequenceMax) {
        introSequence = 255;
        currentScreen = 1; // 0 - server; 1 - office; 2 - settings
      }
      if (introSequence == 1 && hasSave()) {
        introSequence = 26;
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

  // more racks unlocked - message about unlocking more racks
  if (availableRacks > 1) {
    drawCompainionHelp(RacksUnlockedID);
  }

  //unlock AI upgrades
  if (visibleUpgrades > 5) {
    drawCompainionHelp(AIUpgradesUnlockedID);
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
    // od 21 do 25
    if (finalSequence < 255) {
      if (drawCompainionHelp(finalSequence)) {
        companionPopupTimeout = 0;
        companionPopupOpenTimer = 30;
        finalSequence++;
        if (finalSequence > 25) {
          finalSequence = 255;
          currentScreen = 1; // @todo maybe show some final screen or something?
        }
      }
    }
  }

}
