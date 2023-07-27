#pragma once

// File: hiking.h
//
// Hiking module that implements two separate methods to compute the time
// to cross all bridges by hikers.
//

#include <string>
#include <vector>

#include "hiker.h"
#include "bridge.h"


class CHiking {

public:
    CHiking();
    ~CHiking();

    void   addHiker(const SHiker& hiker);       // Add hiker
    void   crossBridge(const SBridge& bridge);  // Cross the bridge
    void   clear();                             // clear internal states
    double getHikeTime() const;                 // Get the total hike time

    enum eComputeType {
        OPTIMIZED        = 1,  // Compute only for the optimized cases
        ALL_COMBINATIONS = 2,  // Enumerate all cases and pick the best timw
    };

    // Set compute type to optimized algo or all_combination algo
    void  setComputeType(eComputeType type);


private:

    // All hikers at any given instance.
    std::vector<SHiker> hikers;

    // Total time taken by hikers to cross all bridges
    double totalTimeToCross;

    eComputeType computeType;

    // ---------------- Approach-1 -----------------//
    // Optimized approach to compute the hike time
    double crossBridgeOptimized(const SBridge& bridge);
    double crossBridgeOptimizedCompute(std::vector<SHiker>& hikers, size_t numHikers, double bridgeLength);


    // ---------------- Approach-2 -----------------//
    // Exhaustive enumerative approach get hike time for all combinations and picking the best time
    enum DIRECTION {
        LEFT_TO_RIGHT = 1,
        RIGHT_TO_LEFT = 2,
    };
    double crossBridgeBruteForce(const SBridge& bridge);
    void   crossBridgeBruteForceCompute(std::vector<SHiker> left, std::vector<SHiker> right,
                                        DIRECTION dir, double cost, double bridgeLength, std::string moveLog);
    // helpers for Approach-2
    std::vector<std::string> leastCostMoveLogs;
    double minCostToMove;   // At the end of all iterations, this has the optimial cost/time to cross bridge
    int    iterations;

    // Utility function to print hikers.
    void printHikers();

};