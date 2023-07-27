#pragma once

// File: bridge.h
//
// Structure to hold bridge details.

#include <string>

struct SBridge {

    std::string name;
    double      length;

    SBridge();
    void clear();
};

