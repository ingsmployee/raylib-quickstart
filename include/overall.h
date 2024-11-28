#pragma once

#include <iostream>
#include <vector>
#include "resource_dir.h"

class MemoryManager {
private:
    std::vector<void *> ram_resources; //this is probably a shit idea
    std::vector<void *> vram_resources;

    int searchPtrVec(std::vector<void *>& mem, void* p) {
        int size = mem.size();
        for (int i = 0; i < size; i++) {
            if (mem.at(i) == p) {
                return i;
            } 
        }
    }

public:
    Image grabImage(const char * location) {
        Image r = LoadImage(location);
        ram_resources.push_back(&r);
        return(r);
    }
    void releaseImage(Image r) {
        UnloadImage(r);

    }


};


class Game:MemoryManager {
private:
    bool shouldQuit;
    Level currentLevel;

public:
    void requestQuit() {

    }

    Level switchToLevel(Level target) {
        
    }
};

class Level: MemoryManager {


public:
    void requestExit() {
        return;
    }
};