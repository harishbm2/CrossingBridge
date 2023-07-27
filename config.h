#pragma once

// File: config.h
//
// Parser for the config file in yaml format
//
// Hiking event has hikers encountering bridges. It will have
// the series of events in the order of happening events lik
// hiker added to the existing group or the group is coming
// across a bridge.
// Example:
//      events: hiking              # Hiking event
//
//      hikers:                     # List of hikers in the group
//        - name: A                 # Name of the hiker
//          speed : 100             # speed in feets/min
//        - name : B
//          speed : 50
//
//      bridge :                    # Encounterd bridge details
//        - name: 1st               # Bridge
//          length : 100            # length in feets
//

#include <string>
#include <fstream>

#include "hiking.h"


// Config class defines parser of the config and trigger the hiking event.
class CConfig {

public:
    CConfig(const std::string& file);
    ~CConfig();

    void setFile(const std::string& file);
    void clearFile();
    void readConfigAndTriggerEvents(CHiking& hiking);

private:
    const std::string defaultConfig = "hiking_event_default.yaml";

    CConfig() = delete;
    void open();
    void close();
    void removeComment(std::string& str);
    void trimString(std::string& str);
    void getTokens(std::string& line, std::vector<std::string>& tokens);

    std::string file;
    std::ifstream fileStream;
};

