#pragma once

#include <iostream>
#include <chrono>
#include <ctime>
#include <map>
#include "raylib.h"

enum debug_mode {
    D_MINIMAL,
    D_EVENTS,
    D_MID,
    D_EXTRA,
    D_RESOURCE,
    D_VERBOSE
};

debug_mode D_MODE = D_RESOURCE;

// time stuff in this function more or less stolen from https://stackoverflow.com/a/27856440
void clog(debug_mode mode, std::string input) { 
    if(mode < D_MODE) {return;}
    std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << sprintf((char*)"[%s]", std::ctime(&time)) << " (" << mode << ") " << input << std::endl;
}

class DebugTimer {
private:
    //note that this is not exact... pretty rough
    std::map<std::string, double> times;
public:
    std::vector<std::string> update() {
        std::map<std::string, double>::iterator begin = times.begin();
        std::map<std::string, double>::iterator end = times.end();

        std::vector<std::string> output;
        output.reserve(0); //will contain nothing on the vast majority of frames

        while(begin != end) {
            if (GetTime() > begin->second) {
                output.push_back(begin->first);
                times.erase(begin);
            }
            begin++;
        }
        return output;
    }

    void startTimer(std::string id, double duration) {
        times.insert(std::pair<std::string, double>(id, GetTime() + duration));
    }
};