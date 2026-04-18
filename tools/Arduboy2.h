// Mock Arduboy2.h for desktop balance tool
#pragma once
#include <cstdint>

// Arduino-style max/min macros (handle mixed types)
#undef max
#undef min
#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))

#define PROGMEM

class Arduboy2 {};
