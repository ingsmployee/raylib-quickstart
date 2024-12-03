#pragma once
#include "overall.h"
#include <vector>
#include "raylib.h"
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
    MATERIAL

}; //well that was a lot

// time stuff in this function inspired by https://stackoverflow.com/a/27856440
void clog(debug_mode mode, std::string input) { 
    if(mode < D_MODE) {return;}
    std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << "[" << std::ctime(&time) << "] (" << mode << ") " << input << std::endl;
}

class Resource { // the beginning of it all muahahaha
protected:
    res_type type;
    char* path;
public:
    res_type getType() {
        return type;
    }
};

class ImageResource: public Resource {
private:
    Image* pointer;
public:
    Image image;
    ImageResource(char* respath) {
        path = respath;
        type = IMAGE;
        load();
    }

    Image* load() {
        image = LoadImage(path);
        pointer = &image;
        return &image;
    }

    void unload() {
        UnloadImage(image);
    }


    ~ImageResource() {
        unload();
    }


};

class Texture2DResource: public Resource {
private:
    Texture2D* t_pointer;
    bool imgPath = false;
    Image* i_pointer;
public:
    Texture2D texture;
    //Overloaded constructor
    //the first definition using char* will first load an image, then load a texture from that image
    //the second definition will load a texture from a given Image object
    //the second mode of operation requires the image to still be around when loading and unloading
    Texture2DResource(char* respath) {
        path = respath;
        type = TEXTURE2D;
        imgPath = true;
        load();
    }

    Texture2DResource(Image fromImage) {
        type = TEXTURE2D;
        i_pointer = &fromImage;
        load();
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
    }

    ~Texture2DResource() {
        unload();
    }
};


class ResourceManager {
private:
    //this is designed to be instantiated per-level, maybe idk
    std::vector<Resource*> resources;
    
public:
    int findResource(Resource* item) {
        for(unsigned int i=0; i < resources.size(); i++) {
            if (item == resources.at(i)) {
                return i;
            }
        }
        return -1;
    }
    void removeRes(Resource* res) {
        int index = findResource(res);
        if (index > -1) {
            resources.erase(resources.begin() + index);
        }
        else {
            clog(D_RESOURCE, "Could not find resource in ResourceManager. Are you checking the right one?"); //this is asswater
        }
    }

    template <class T> 
    T* pload(const char* arg) {
        T* item = new T((char*)arg);
        resources.push_back(item);
        return item;
        
    }

    template<class T, typename Y>
    T* pload(Y arg) {
        T* item = new T(arg);
        resources.push_back(item);
        return item;
    }

    void push(Resource* res) {
        resources.push_back(res);
    }

    // uses delete on every pointer in resources
    void clearAll() {
        for(Resource* res : resources) {
            delete res; //hoping this invokes the destructor. probably does. hopefully
        }
        resources.clear();
    }




};










// OLD MEMORY MANAGER USING CRAPPY VOID* INSTEAD OF BASED RESOURCE*
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