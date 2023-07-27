// File: config.cpp
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

#include <iostream>
#include <vector>
#include <sstream>
#include <cassert>

#include "config.h"

CConfig::CConfig(const std::string& file) {
    this->file = file;
}

CConfig::~CConfig() {
    if (fileStream.is_open()) {
        fileStream.close();
    }
}

void CConfig::setFile(const std::string& file) {
    this->file = file;
}
void CConfig::clearFile() {
    this->file.clear();
}

void CConfig::open() {
    if (fileStream.is_open()) {
        fileStream.close();
    }
    fileStream.open(file);
}

void CConfig::close() {
    if (fileStream.is_open()) {
        fileStream.close();
    }
}

// Read the configuration file and trigger events to cross the bridge.
void CConfig::readConfigAndTriggerEvents(CHiking& hiking) {

    if (file.size()) {

        if (!fileStream.is_open()) {
            fileStream.open(file);

        }
        if (fileStream.is_open()) {

            SHiker  hiker;
            SBridge bridge;

            enum EType {
                NONE = 0,
                HIKER = 1,
                BRIDGE = 2,
            } eType(NONE);

            while (fileStream) {
                std::string line;
                getline(fileStream, line);

                // Erase the comment
                removeComment(line);

                std::vector<std::string> tokens;
                getTokens(line, tokens);

                if (tokens.size()) {
                    if (tokens[0] == "hikers") {
                        eType = HIKER;
                        bridge.clear();
                        continue;
                    }
                    else if (tokens[0] == "bridge") {
                        eType = BRIDGE;
                        hiker.clear();
                        continue;
                    }
                }

                if (tokens.size() == 2) {
                    switch (eType) {
                    case HIKER:
                        if (tokens[0] == "name") {
                            hiker.name = tokens[1];
                        }
                        else if (tokens[0] == "speed") {
                            try {
                                hiker.speed = std::stod(tokens[1]);
                                hiking.addHiker(hiker);
                            }
                            catch (std::invalid_argument& ex) {
                                std::cerr << "Invalid input for speed, unable to convert to double." << std::endl;
                            }
                            catch (std::out_of_range& ex) {
                                std::cerr << "Value is out of the range for speed." << std::endl;
                            }
                        }
                        break;
                    case BRIDGE:
                        if (tokens[0] == "name") {
                            bridge.name = tokens[1];
                        }
                        else if (tokens[0] == "length") {
                            try {
                                bridge.length = std::stod(tokens[1]);
                                hiking.crossBridge(bridge);
                            }
                            catch (std::invalid_argument& ex) {
                                std::cerr << "Invalid input for length, unable to convert to double." << std::endl;
                            }
                            catch (std::out_of_range& ex) {
                                std::cerr << "Value is out of the range for length." << std::endl;
                            }
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
            fileStream.close();
        }
        else {
            std::cerr << "Unable to open the file: " << file << ", check the permission to access." << std::endl;
        }
    }
    else {
        std::cerr << "Invalid config file." << std::endl;
    }
}

// Helper function to remove comments in config file
void CConfig::removeComment(std::string& str) {
    size_t hashPos = str.find('#');
    if (hashPos != std::string::npos) {
        str = str.substr(0, hashPos);
    }
}

// Helper function to trim leading and trailing whitespaces from the token
void CConfig::trimString(std::string& str) {
    size_t firstCharPos = str.find_first_not_of(" \t\n\r");
    size_t lastCharPos  = str.find_last_not_of(" \t\n\r");
    if (firstCharPos != std::string::npos && lastCharPos != std::string::npos) {
        str = str.substr(firstCharPos, lastCharPos - firstCharPos + 1);
    }
}

// Helper funtion to get tokens in a config line in yaml.
void CConfig::getTokens(std::string& line, std::vector<std::string>& tokens) {
    std::stringstream ss(line);
    std::string token;

    tokens.clear();

    bool bFirstToken = true;
    while (std::getline(ss, token, ':')) {

        if (bFirstToken) {

            size_t dashPos = token.find('-');
            if (dashPos != std::string::npos) {
                token = token.substr(dashPos+1, token.length());
            }
            bFirstToken = false;
        }
        // remore leading and trailing spaces
        trimString(token);
        tokens.push_back(token);
    }
}

