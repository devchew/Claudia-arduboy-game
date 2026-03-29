#pragma once
#include <Arduboy2.h>
#include "State.h"

extern Arduboy2 arduboy;

void onBuy() {

}

void tickUpdate() {
    
    totalCapacity = 0;
    money = 1;
    inbound = 1;
    loadPercent = 70;
}