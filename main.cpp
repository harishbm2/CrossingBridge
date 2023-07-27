// File: main.cpp
//
// Create the objects for computing the hiking time and parsing the configuration
// that has details of hikers and bridges.
//
// Two different approaches to get the hiking time is defined in hiking object.
// One method gets the best time by computing time for all combinations of hikers
// for the bridge crosses and the other method computes by using optimal strategy.
//
// At the beginiing, we compare the output from both the methods on couple of
// sample configs which has smaller set. If we get the same result from both the
// methods then, it indicates our understanding of the solution to the problem
// is assumed to be correct.
// After this check is passed, we can use only the optimal approach for the
// subsequent configs which may have larger set.

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <sstream>
#include <chrono>

#include "hiking.h"
#include "config.h"
#include "debug.h"

// Trigger computation using the optimal approach
double crossBridgeOptimizedApproach(const std::string& configFile) {
    CHiking hiking;
    hiking.setComputeType(CHiking::OPTIMIZED);

    CConfig confObj(configFile);
    confObj.readConfigAndTriggerEvents(hiking);

    return hiking.getHikeTime();
}

// Trigger the computation by enumerating all combinations of hikers
double crossBridgeAllCombinations(const std::string& configFile) {
    CHiking hiking;
    hiking.setComputeType(CHiking::ALL_COMBINATIONS);

    CConfig confObj(configFile);
    confObj.readConfigAndTriggerEvents(hiking);

    return hiking.getHikeTime();
}

// This function is used to validate if both the approaches get the same result.
// This way we can be sure of the approaches and use only the optimal one for testing at
// other times.
// Exhaustive combination Approach-2 is exponential in nature in terms of complexity.
// Hence it should be avoided.
bool validateBridgeCrossAlgosGiveSameResult() {

    std::vector<std::string> testFiles = { "bridge_cross_1.yaml", "bridge_cross_2.yaml" };
    for (auto testFile : testFiles) {

        auto start_1 = std::chrono::high_resolution_clock::now();
        double hikeTime_from_optimized_approach = crossBridgeOptimizedApproach(testFile);
        auto end_1 = std::chrono::high_resolution_clock::now();
        auto duration_1 = std::chrono::duration_cast<std::chrono::microseconds>(end_1 - start_1);

        auto start_2 = std::chrono::high_resolution_clock::now();
        double hikeTime_from_all_combinations = crossBridgeAllCombinations(testFile);
        auto end_2 = std::chrono::high_resolution_clock::now();
        auto duration_2 = std::chrono::duration_cast<std::chrono::microseconds>(end_2 - start_2);

        if (bIsDebug(DEBUG_CLOCK)) {
            std::cout << "total hike time-1: " << hikeTime_from_optimized_approach << ", it took: " << duration_1.count() << " ms to compute" << std::endl;
            std::cout << "total hike time-2: " << hikeTime_from_all_combinations << ", it took: " << duration_2.count() << " ms to compute" << std::endl;
        }

        if (hikeTime_from_optimized_approach != hikeTime_from_all_combinations) {
            std::cout << "Debug, as we get different results from different approaches.";
            return false;
        }
    }
    // This means both approaches give the same result, hence we can use optimal approach going forward.
    return true;
}

// Get the total hiking time to cross all bridges using optimal method.
void getTimeTakenByHikersToCrossAllBridges(const std::string& configFile) {

    auto start_1 = std::chrono::high_resolution_clock::now();

    double hikeTime_from_optimized_approach = crossBridgeOptimizedApproach(configFile);

    auto end_1 = std::chrono::high_resolution_clock::now();
    auto duration_1 = std::chrono::duration_cast<std::chrono::microseconds>(end_1 - start_1);

    if (bIsDebug(DEBUG_CLOCK)) {
        std::cout << "total hike time: " << hikeTime_from_optimized_approach << ", it took: " << duration_1.count() << " ms to compute" << std::endl;
    }

    std::cout << "Total time taken to hike: " << hikeTime_from_optimized_approach << std::endl;
}



// Main:
// Caller may pass config file (yaml) as argument in the command line.
// Otherwise, the default one will be used.

int main(int argc, char* argv[]) {

    // Based on need enable/disable the flags to debug.
    //
    //setDebugLevels(DEBUG_TRACE | DEBUG_WARNING | DEBUG_INFO | DEBUG_ERROR | DEBUG_INTER | DEBUG_STEPS);
    //setDebugLevels(DEBUG_INTER);

    if (!validateBridgeCrossAlgosGiveSameResult()) {
        std::cerr << "exiting as algo is not giving correct output yet" << std::endl;
        return -1;
    }

    std::string configFile;
    if (argc == 2) {
        configFile = argv[1];
    }
    else {
        configFile = "hiking_event_default.yaml";
    }

    getTimeTakenByHikersToCrossAllBridges(configFile);

    return 0;
}
