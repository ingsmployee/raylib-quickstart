#pragma once
#include "overall.h"
#include <vector>
#include "raylib.h"
#include <chrono>
#include <ctime>

enum debug_mode {
    D_NONE,
    D_LOW,
    D_VERBOSE
};

enum mem_type {
    RAM_TYPE,
    VRAM_TYPE,
    ANY_TYPE
};
enum res_type { //organized for clarity. doesn't really matter
    //ram
    RAM_START,
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
    VRAM_START,
    TEXTURE2D,
    RENDER_TEXTURE,
    SHADER,
    FONT,
    MODEL_ANIMATION, //gpu???
    MODEL_ANIMATION_ARRAY,

    //either
    ANY_START,
    MODEL,
    MESH,
    MATERIAL,

}; //well that was a lot

// time stuff in this function inspired by https://stackoverflow.com/a/27856440
void log(debug_mode mode, std::string input) { 
    std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << "[" << std::ctime(&time) << "] (" << mode << ") " << input << std::endl;
}


class ObjMemoryManager {




};

class Resource {
public:
    res_type type;
    char* path;
};

class ImageResource: Resource {
private:
    Image* pointer;
public:
    Image image;
    ImageResource(char* respath) {
        path = respath;
        type = IMAGE;
    }

    Image* load() {
        image = LoadImage(path);
        pointer = &image;
        return &image;
    }

    void unload() {
        UnloadImage(image);
        delete pointer;
    }


    ~ImageResource() {
        unload();
    }


};

class Texture2DResource: Resource {
private:
    Texture2D* t_pointer;
    bool imgPath = false;
    Image* i_pointer;
public:
    Texture2D texture;
    //overload the constructor
    //respath will load the image then the texture
    //fromImage will just load the image argument
    Texture2DResource(char* respath) {
        path = respath;
        type = TEXTURE2D;
        imgPath = true;
    }

    Texture2DResource(Image fromImage) {
        type = TEXTURE2D;
        i_pointer = &fromImage;
    }

    Texture2D* load() {
        if (imgPath) {
            Image image = LoadImage(path); //avoiding a memory leak
            texture = LoadTextureFromImage(image);
            UnloadImage(image);
        }
        else {
            texture = LoadTextureFromImage(*i_pointer);
        }
        t_pointer = &texture;
        return t_pointer;
    }

    void unload() {
        UnloadTexture(texture);
        delete t_pointer;
        // don't delete i_pointer
        // because the image is not actually stored in this
    }

    ~Texture2DResource() {
        unload();
    }
};







// OLD VERSION USING CRAPPY VOID*
class VoidPtrMemoryManager {
// this class serves to abstract a lot of the loading and unloading methods,
//      essentially working as a sort-of dictionary to keep track of resources at a local level,
//      allowing for complicated screens/levels to be unloaded completely and safely
// since the vectors only hold enums and pointers, they probably won't take up nearly
//      as much memory as the resources themselves

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

    mem_type getMemType(res_type r) {
        if (r > VRAM_START) {
            if (r > ANY_START) {
                return ANY_TYPE;
            }
            else {
                return VRAM_TYPE;
            }
        }
        else {
            return RAM_TYPE;
        }
    }

    // doesn't touch the vectors. erases a resource using raylib stuff
    void unloadResOfType(void* r, res_type resType) {
        switch(resType) {
            case IMAGE:
                UnloadImage(*(Image*)r);
            break;
            case TEXTURE2D:
                UnloadTexture(*(Texture*)r);
            break;
        }
    }

    // takes an index then the memory vectors directly
    // erases that entry in the vectors and with raylib erase
    void unloadResAtIndex(unsigned int index, std::vector<void*>& mem, std::vector<res_type>& memType) {
        if (mem.size() < 1) {
            return;
        }

        mem.erase(mem.begin() + index); //remove from the mem vectors
        memType.erase(memType.begin() + index); 

        unloadResOfType(mem.at(index), memType.at(index)); //use raylib function to actually remove it
    }

    void unloadMemVec(std::vector<void*> &mem, std::vector<res_type> &memType) {
        for(int i = 0; i < mem.size(); i++) {
            unloadResAtIndex(i, mem, memType);
        }
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

    void showVRAM() {
        int i = 0;
        std::cout << "VRAM (" << vram_resources.size() << " entries): {";
        for (void* r: vram_resources) {
            std::cout << "{(" << vram_restypes.at(i) << ") " << r << "} , ";

            i++;
        }
        std::cout << "}" << std::endl;
    }

    void registerResource(void* res, res_type resType) { 
        if (resType > RAM_START && resType < VRAM_START) {
            ram_resources.push_back(res);
            ram_restypes.push_back(resType);
        } else if (resType > VRAM_START && resType < ANY_START) {
            vram_resources.push_back(&res);
            vram_restypes.push_back(resType);
        }
    }

    // identifies the type of memory to remove from, then removes from that memory
    // different to unloadResOfType() because it also finds the memory type and erases accordingly
    void unloadResource(void* r, res_type resType) {


        // identify what type of memory
        std::vector<void*>* resVectorPtr;
        std::vector<res_type>* resTypeVectorPtr;
        const mem_type memType = getMemType(resType);
        switch(memType) {
            case RAM_TYPE:
                resVectorPtr = &ram_resources;
                resTypeVectorPtr = &ram_restypes;
                break;
            case VRAM_TYPE:
                resVectorPtr = &vram_resources;
                resTypeVectorPtr = &vram_restypes;
                break;
        }

        if ((*resVectorPtr).size() < 1) {
            return;
        }

        // clear that kind of memory
        // TODO: make the algorithm more efficient by searching res_type first, then searching through said res_type. could just be a simple &&
        int index = searchPtrVec(*resVectorPtr, &r);

        unloadResAtIndex(index, *resVectorPtr, *resTypeVectorPtr);
    }
    
    void clearAll() {
        clearMemory(RAM_TYPE);
        clearMemory(VRAM_TYPE);
    }



    void clearMemory(mem_type memType) {
        switch (memType) {
            case RAM_TYPE:
                unloadMemVec(ram_resources, ram_restypes);
            break;
            case VRAM_TYPE:
                unloadMemVec(vram_resources, vram_restypes);
            break;
        }
    }

};