#pragma once

// File: debug.h
// Debug levels for controlling the debug/trace prints.

extern unsigned int g_debug; // For trace/debug/etc

#define DEBUG_TRACE     (0x1 << 8)
#define DEBUG_WARNING   (0x1 << 7)
#define DEBUG_INFO      (0x1 << 6)
#define DEBUG_ERROR     (0x1 << 5)
#define DEBUG_INTER     (0x1 << 4)
#define DEBUG_STEPS     (0x1 << 3)
#define DEBUG_CLOCK     (0x1 << 2)

void setDebugLevels(unsigned int levels);
bool bIsDebug(unsigned int level);