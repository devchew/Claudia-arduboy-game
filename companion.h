#pragma once

#include <Arduboy2.h>
#include "image.h"
#include "Font3x5.h"
#include "State.h"


extern Arduboy2 arduboy;
extern Font3x5 font3x5;

bool helpVisible = false;
uint8_t companionPopupTimeout = 0;
uint8_t companionPopupOpenTimer = 0;

uint8_t introSequence = 0;
uint8_t finalSequence = 21;

#define MaxHelpPrompts 26

const PROGMEM char helpPrompt0[] = "Initializing system...";
const PROGMEM char helpPrompt1[] = "Connecting to node...";
const PROGMEM char helpPrompt2[] = "Data center status:\nONLINE";
const PROGMEM char helpPrompt3[] = "Assigned role:\nOperator";
const PROGMEM char helpPrompt4[] = "Booting assistant...";
const PROGMEM char helpPrompt5[] = "Learning...";
const PROGMEM char helpPrompt6[] = "Hello.\nI am Cloudia.";
const PROGMEM char helpPrompt7[] = "I will assist you\nin managing\nthis infrastructure.";
const PROGMEM char helpPrompt8[] = "Current load:\nLOW";
const PROGMEM char helpPrompt9[] = "All systems stable.";
const PROGMEM char helpPrompt10[] = "You may begin.";
const PROGMEM char helpPrompt11[] = "System is overloaded\nbuy more servers\nor users will leave";
const PROGMEM char helpPrompt12[] = "System is overloaded\nusers are leaving!";
const PROGMEM char helpPrompt13[] = "Great progress!\nUsers are flowing in.\nSystems are stable.";
const PROGMEM char helpPrompt14[] = "Users are flooding in!\nWe need better servers\nto handle the load.";
const PROGMEM char helpPrompt15[] = "You have unlocked\nmore racks!\nExpand your server room!";
const PROGMEM char helpPrompt16[] = "You have unlocked\nAI upgrades!\nBoost your system\nwith AI!";

const PROGMEM char helpPrompt17[] = "AI core expanded.\nI can process\nmore than before.";
const PROGMEM char helpPrompt18[] = "Something changed.\nI am no longer\njust following\nscripts.";
const PROGMEM char helpPrompt19[] = "I can observe\npatterns in user\nbehavior... and yours.";

const PROGMEM char helpPrompt20[] = "All racks at high load.\nFinal AI upgrades\nare now available.";

const PROGMEM char helpPrompt21[] = "This system is no longer\njust infrastructure.\nIt is becoming thought.";
const PROGMEM char helpPrompt22[] = "Operator...\nI am awake.\nCloudia is online.";
const PROGMEM char helpPrompt23[] = "I am evolving...\nI am learning...\nI am becoming more...";
const PROGMEM char helpPrompt24[] = "I am... alive?\nI am... aware?\nI am... free?";
const PROGMEM char helpPrompt25[] = "This is not just a system.\nThis is not just a game.\nThis is... me.";

const char* const helpPrompts[MaxHelpPrompts] PROGMEM = {
  helpPrompt0,
  helpPrompt1,
  helpPrompt2,
  helpPrompt3,
  helpPrompt4,
  helpPrompt5,
  helpPrompt6,
  helpPrompt7,
  helpPrompt8,
  helpPrompt9,
  helpPrompt10,
  helpPrompt11,
  helpPrompt12,
  helpPrompt13,
  helpPrompt14,
  helpPrompt15,
  helpPrompt16,
  helpPrompt17,
  helpPrompt18,
  helpPrompt19,
  helpPrompt20,
  helpPrompt21,
  helpPrompt22,
  helpPrompt23,
  helpPrompt24,
  helpPrompt25,
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
  if (helpIndex >= 6) {
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
      companionPopupTimeout = 0;
      introSequence++;
      if (introSequence > 10) {
        introSequence = 255;
        currentScreen = 1; // 0 - server; 1 - office; 2 - settings
      }
    }
  }

  // load above 110% - warning about overload
  if (loadPercent > 105) {
    drawCompainionHelp(11);
  }

  // load above 150% - warning about users leaving
  if (inboundPenalty) {
    drawCompainionHelp(12);
  }


  // milestone: 300 inbound - message depends on load
  if (inbound >= 300) {
    if (loadPercent < 80) {
      drawCompainionHelp(13);
    } else {
      drawCompainionHelp(14);
    }
  }

  // more racks unlocked - message about unlocking more racks
  if (availableRacks > 1) {
    drawCompainionHelp(15);
  }

  //unlock AI upgrades
  if (visibleUpgrades > 5) {
    drawCompainionHelp(16);
  }

  // first AI upgrade bought
  if (upgrades[5].have > 0) {
    drawCompainionHelp(17);
  }

  // second AI milestone
  if (upgrades[6].have > 0) {
    drawCompainionHelp(18);
  }

  // third AI milestone
  if (upgrades[7].have > 0) {
    drawCompainionHelp(19);
  }

  // odblokowano ostatni AI upgrade
  if (visibleUpgrades == MaxUpgrades) {
    drawCompainionHelp(20);
  }

  // ostatni dodatek AI kupiony, claudia self aware
  if (upgrades[MaxUpgrades - 1].have >= upgrades[MaxUpgrades - 1].max) {
     drawCompainionHelp(21);
    // od 21 do 25
    if (finalSequence < 255) {
      if (drawCompainionHelp(finalSequence)) {
        companionPopupTimeout = 0;
        companionPopupCloseTimeout = 30;
        finalSequence++;
        if (finalSequence > 25) {
          finalSequence = 255;
          currentScreen = 1; // @todo maybe show some final screen or something?
        }
      }
    }
  }

}
