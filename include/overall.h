#pragma once

#include <iostream>
#include <vector>

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
        ram_resources.erase(ram_resources.begin() + searchPtrVec(ram_resources, &r));
    }

    void exposeMemory() {
        std::cout << "RAM: {";
        for (void* r: ram_resources) {
            std::cout << r << ", ";
        }
        std::cout << "}" <<std::endl;
    }
};

class Level {
private:
    MemoryManager mem = MemoryManager();
public:
    int gragagono;
    void requestExit() {
        return;
    }
};

class Game {
private:
    MemoryManager mem = MemoryManager();
    bool shouldQuit;
    Level currentLevel;

public:
    void requestQuit() {
        shouldQuit = true;
    }

    Level switchToLevel(Level target) {
        return target;
    }
};