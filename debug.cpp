// File: debug.h
// Set debug levels for controlling the debug/trace prints.

#include "debug.h"

unsigned int g_debug = 0; // For trace/debug/etc

void setDebugLevels(unsigned int levels) {
    g_debug = levels;
}

bool bIsDebug(unsigned int level) {
    return g_debug & level;
}
