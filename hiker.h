#pragma once

// File: hiker.h
//
// Structure to hold hiker details.

#include <string>

// Hiker details
struct SHiker {

    std::string name;
    double      speed;

    SHiker();
    SHiker(std::string name, double speed);

    bool operator < (const SHiker& hiker);
    void clear();
};
