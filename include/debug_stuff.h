#pragma once

#include <iostream>
#include <chrono>
#include <ctime>

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