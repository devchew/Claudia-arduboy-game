#pragma once

#include <Arduboy2.h>
#include "Font3x5.h"
#include "State.h"

extern Arduboy2 arduboy;
extern Font3x5 font3x5;


uint8_t animationFrame = 0;

void screenTurnOffAnimation() {
    if (animationFrame <= 32) {
        // Phase 1: two horizontal lines converging vertically to center
        arduboy.fillRect(0, 0, 128, animationFrame, BLACK);
        arduboy.fillRect(0, 64 - animationFrame, 128, animationFrame, BLACK);
        arduboy.drawFastHLine(0, animationFrame, 128, WHITE);
        arduboy.drawFastHLine(0, 63 - animationFrame, 128, WHITE);
    } else if (animationFrame <= 96) {
        // Phase 2: single line shrinking horizontally from sides to center
        uint8_t hFrame = animationFrame - 32;
        uint8_t lineStart = hFrame * 2;
        uint8_t lineEnd = 128 - hFrame * 2;
        arduboy.fillRect(0, 0, 128, 64, BLACK);
        if (lineStart < lineEnd) {
            arduboy.drawFastHLine(lineStart, 31, lineEnd - lineStart, WHITE);
            arduboy.drawFastHLine(lineStart, 32, lineEnd - lineStart, WHITE);
        }
    } else {
        // Phase 3: "thank you" text on black screen
        arduboy.fillRect(0, 0, 128, 64, BLACK);
        font3x5.setCursor(46, 30);
        font3x5.print(F("thank you"));
    }

    if (animationFrame <= 97) {
        animationFrame++;
    }
}