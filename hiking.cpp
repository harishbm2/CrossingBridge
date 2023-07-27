// File: hiking.cpp
//
// Two different approaches to get the hiking time is defined in hiking object.
// One method gets the best time by computing time for all combinations of hikers
// for the bridge crosses and the other method computes by using optimal strategy.

#include <iostream>
#include <algorithm>
#include <sstream>
#include <climits>
#include <cassert>

#include "hiking.h"
#include "debug.h"


CHiking::CHiking() : totalTimeToCross(0), computeType(OPTIMIZED),
                     minCostToMove(INT_MAX), iterations(0)
{
}

CHiking::~CHiking() {
}

void CHiking::clear() {
    totalTimeToCross = 0;
    hikers.clear();
}

double CHiking::getHikeTime() const {
    return totalTimeToCross;
}

void CHiking::setComputeType(eComputeType type) {
    computeType = type;
}

// Add hiker to the hiker group. Whenever bridge is encountered
// at that time, cross bridge function is executed.
void CHiking::addHiker(const SHiker& hiker) {
    if (bIsDebug(DEBUG_TRACE)) {
        std::cout << "ADD:   " << "Hiker: " << hiker.name << ", Speed: " << hiker.speed << " feet/minute" << std::endl;
    }
    hikers.push_back(hiker);
}

// Whenever bridge is encounterd, cross bridge functrion is executed.
void CHiking::crossBridge(const SBridge& bridge) {

    if (bIsDebug(DEBUG_TRACE)) {
        std::cout << "CROSS: " << "Bridge: " << bridge.name << ", Length: " << bridge.length << " feet" << std::endl;
    }
    if (computeType == OPTIMIZED) {
        crossBridgeOptimized(bridge);
    }
    else if (computeType == ALL_COMBINATIONS) {
        crossBridgeBruteForce(bridge);
    }
}


// ---------------- Approach-1 -----------------//
// This is the optimal approach. It uses the following strategies to get the best time.
// In some case first method gives best results and in some other cases second method
// gives best results. So, we get the minimum of these methods.
//
// e.g.1:
//     Hikers: A:10, B:20, C:40, D:50
//          Left  :    Bridge      : Right
//         A,B,C,D: -------------- :
//         C,D    : ---A,B(20)---> :         Total:  20
//         C,D    : <-- A (10)---- :  B      Total:  30
//         A      : ---C,D(50)---> :  B      Total:  80
//         A      : <-- B (20)---- : C,D     Total: 100
//                : -- A,B(20)---> : C,D     Total: 120
//                : ---------------: A,B,C,D Total: 120
//
//
// e.g.2:
//     Hikers: A:10, B:120, C:140, D:150
//          Left  :    Bridge      : Right
//         A,B,C,D: -------------- :
//         B,C    : ---A,D(150)--> :         Total: 150
//         B,C    : <-- A (10)---- :  D      Total: 160
//         B      : ---A,C(140)--> :  D      Total: 300
//         B      : <-- A (10)---- : C,D     Total: 310
//                : -- A,B(120)--->: C,D     Total: 430
//                : ---------------: A,B,C,D Total: 430
//

double CHiking::crossBridgeOptimized(const SBridge& bridge) {
    // Sort the hikers for the optimized technquie.
    std::sort(hikers.begin(), hikers.end());

    printHikers();

    double timeToCross = crossBridgeOptimizedCompute(hikers, hikers.size(), bridge.length);
    totalTimeToCross += timeToCross;

    if (bIsDebug(DEBUG_INTER)) {
        std::cout << "Approach-1: Time to cross bridge " << bridge.name << ": " << timeToCross << std::endl;
        std::cout << "Approach-1: Total Time to cross: " << totalTimeToCross << std::endl;
    }

    return totalTimeToCross;
}

double CHiking::crossBridgeOptimizedCompute(std::vector<SHiker>&hikers, size_t numHikers, double bridgeLength) {

    // For one or two hikers, it will be the time required for the slowest hiker
    if (numHikers < 3) {
        return bridgeLength / hikers[1].speed;
    }
    // For three hikers, it will be the time required for all hikers
    else if (numHikers == 3) {
        return bridgeLength / hikers[0].speed
            + bridgeLength / hikers[1].speed
            + bridgeLength / hikers[2].speed;
    }
    // If four or more hikers, compute based on the optimized cases
    else {
        double timeForCase1 = bridgeLength / hikers[1].speed + bridgeLength / hikers[0].speed
            + bridgeLength / hikers[numHikers - 1].speed + bridgeLength / hikers[1].speed;
        double timeForCase2 = bridgeLength / hikers[numHikers - 1].speed + bridgeLength / hikers[0].speed
            + bridgeLength / hikers[numHikers - 2].speed + bridgeLength / hikers[0].speed;
        double minTime = std::min(timeForCase1, timeForCase2);

        // Compute for remaining hikers
        return minTime + crossBridgeOptimizedCompute(hikers, numHikers - 2, bridgeLength);
    }
}



// ---------------- Approach-2 -----------------//
// This is the exhaustive approach, enumerates all combinations of the hikers crossing
// the bridge and takes the best time out of it.
// This exhaustive enumberation comes in handy to understand in development phase and
// make sure the optimized one is actually the correct one.

double CHiking::crossBridgeBruteForce(const SBridge& bridge) {

    std::vector<SHiker> left = hikers;
    std::vector<SHiker> right;
    double cost = 0;
    std::string moveLog;
    minCostToMove = INT_MAX;
    iterations = 0;

    printHikers();

    crossBridgeBruteForceCompute(left, right, LEFT_TO_RIGHT, cost, bridge.length, moveLog);

    totalTimeToCross += minCostToMove;

    if (bIsDebug(DEBUG_STEPS)) {
        std::cout << "Approach-2: Total iterations: " << iterations << std::endl;
        for (auto leastCostMoveLog : leastCostMoveLogs) {
            std::cout << leastCostMoveLog << std::endl;
        }
    }

    if (bIsDebug(DEBUG_INTER)) {
        std::cout << "Approach-2: Time to cross bridge " << bridge.name << ": " << minCostToMove << std::endl;
        std::cout << "Approach-2: Total Time to cross: " << totalTimeToCross << std::endl;
    }

    return totalTimeToCross;
}


void CHiking::crossBridgeBruteForceCompute(std::vector<SHiker> left, std::vector<SHiker> right,
                                           DIRECTION dir, double cost, double bridgeLength, std::string moveLog) {

    if (left.size() == 0) {
        iterations++;
        std::stringstream ss;
        ss << moveLog << "Total cost: " << cost << std::endl;

        if (minCostToMove > cost) {
            minCostToMove = cost;
            leastCostMoveLogs.clear();
        }
        if (minCostToMove == cost) {
            leastCostMoveLogs.push_back(moveLog);
        }
        return;
    }

    // prune/backtrack if the cost is greater than already computed smaller one.
    // If this prune is removed, all cases can be seen.
    if (minCostToMove <= cost) {
        return;
    }

    switch (dir) {
    case LEFT_TO_RIGHT:
    {
        // For ecah pair in left, get the max and adjust the cost
        // update the left and right accordingly.
        // Here get all combination of the pairs to cross the bridge (left to right)

        for (int i = 0; i < left.size() - 1; ++i) {
            for (int j = i + 1; j < left.size(); ++j) {
                double legCost = std::max(bridgeLength / left[i].speed, bridgeLength / left[j].speed);

                std::vector<SHiker> leftUpdated;
                for (int k = 0; k < left.size(); ++k) {
                    if (k != i && k != j) {
                        leftUpdated.push_back(left[k]);
                    }
                }
                std::stringstream ss;
                ss << moveLog << "    ";
                for (auto hiker : left) { ss << " " << hiker.name; }
                ss << " -- " << left[i].name << ", " << left[j].name << " (" << (legCost) << ") --> ";

                std::vector<SHiker> rightUpdated = right;
                rightUpdated.push_back(left[i]);
                rightUpdated.push_back(left[j]);

                for (auto hiker : rightUpdated) { ss << " " << hiker.name; }
                ss << ", currentCost: " << cost + legCost << std::endl;

                crossBridgeBruteForceCompute(leftUpdated, rightUpdated, RIGHT_TO_LEFT, cost + legCost, bridgeLength, ss.str());
            }
        }
    }
    break;

    case RIGHT_TO_LEFT:
    {
        // Here enumerate for all users to cross the bridge (right to left)
        for (int i = 0; i < right.size(); ++i) {
            double legCost = bridgeLength / right[i].speed;

            std::vector<SHiker> rightUpdated = right;
            rightUpdated.erase(rightUpdated.begin() + i);

            std::vector<SHiker> leftUpdated = left;
            leftUpdated.push_back(right[i]);

            std::stringstream ss;
            ss << moveLog << "    ";
            for (auto hiker : leftUpdated) { ss << " " << hiker.name; }
            ss << " <-- " << right[i].name << " (" << (legCost) << ") -- ";

            for (auto hiker : rightUpdated) { ss << " " << hiker.name; }
            ss << ", currentCost: " << cost + legCost << std::endl;

            crossBridgeBruteForceCompute(leftUpdated, rightUpdated, LEFT_TO_RIGHT, cost + legCost, bridgeLength, ss.str());
        }
    }
    break;
    default:
        assert(0); // Should not come here!
        break;
    }
}

void CHiking::printHikers() {
    if (bIsDebug(DEBUG_TRACE)) {
        std::cout << "Hikers:";
        for (auto hiker : hikers) {
            std::cout << " (" << hiker.name << ":" << hiker.speed << ")";
        }
        std::cout << std::endl;
    }
}
