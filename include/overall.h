#pragma once

#include <iostream>
#include <vector>

enum mem_type {
    RAM_TYPE,
    VRAM_TYPE
};
enum res_type { //organized for clarity. doesn't really matter
    //ram
    IMAGE,
    NPATCHINFO, //ram?
    GLYPH_INFO, //font info
    RANDOM_SEQUENCE,
    FILE_DATA,
    FILE_TEXT,
    DIR_FILES,
    DROPPED_FILES,
    AUTOAMTION_EVENT_LIST,
    IMAGE_COLORS,
    IMAGE_PALETTE,
    WAVE,
    SOUND,
    SOUND_ALIAS,
    WAVE_SAMPLES,
    MUSIC_STREAM,
    AUDIO_STREAM,


    //vram
    TEXTURE,
    RENDER_TEXTURE,
    SHADER,
    FONT,
    MODEL_ANIMATION, //gpu???
    MODEL_ANIMATION_ARRAY,

    //either
    MODEL,
    MESH,
    MATERIAL,

}; //well that was a lot

class MemoryManager {
private:
    std::vector<void *> ram_resources; //this is probably a shit idea
    std::vector<void *> vram_resources;
    std::vector<res_type> ram_restypes;
    std::vector<res_type> vram_restypes;

    int searchPtrVec(std::vector<void *>& mem, void* p) {
        int size = mem.size();
        for (int i = 0; i < size; i++) {
            if (mem.at(i) == p) {
                return i;
            } 
        }
        return 0;
    }

public:
    void showRAM() {
        int i = 0;
        std::cout << "RAM (" << ram_resources.size() << " entries): {";
        for (void* r: ram_resources) {
            std::cout << "{(" << ram_restypes.at(i) << ") " << r << "} , ";

            i++;
        }
        std::cout << "}" << std::endl;
    }

    void registerResource(void* res, res_type resType, mem_type memType) { // TODO: fix this lmao
        if (memType = RAM_TYPE) {
            ram_resources.push_back(res);
            ram_restypes.push_back(resType);
        } else {
            vram_resources.push_back(&res);
            vram_restypes.push_back(resType);
        }
    }

    void unloadResource(void* r, res_type resType) {
        int index = searchPtrVec(ram_resources, &r);
        ram_resources.erase(ram_resources.begin() + index);
        ram_restypes.erase(ram_restypes.begin() + index);
        switch(resType) {
            case IMAGE:
                UnloadImage(*(Image*)r);
            break;
            case TEXTURE:
                UnloadTexture(*(Texture2D*)r);
            break;
        }
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