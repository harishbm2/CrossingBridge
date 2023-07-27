// File: hiker.cpp
//
// Structure to hold hiker details.

#include "hiker.h"

SHiker::SHiker() : speed(0) {}

SHiker::SHiker(std::string name, double speed) : name(name), speed(speed) {}

// Comarator function used to sort the hiker vector array.
// sort based on the high speed hiker to low speed hiker.
bool SHiker::operator < (const SHiker& hiker) {
    // higher the speed, lesser the time to cross the bridge
    return speed > hiker.speed;
}

void SHiker::clear() {
    name.clear();
    speed = 0;
}

