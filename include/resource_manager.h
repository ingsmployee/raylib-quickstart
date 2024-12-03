#pragma once

#include "debug_stuff.h"
#include <iostream>
#include "raylib.h"
#include <vector>

#include "resource_base.h"






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
    void removeResource(Resource* res) {
        int index = findResource(res);
        if (index > -1) {
            resources.erase(resources.begin() + index);
        }
        else {
            clog(D_RESOURCE, "Could not find resource in ResourceManager. Are you checking the right one?"); //this is asswater
        }
    }

    //instantiates and pushes resource to manager, returning a pointer for that resource
    //example usage:
    //Texture2DResource* tex = rema.pload<Texture2DResource> ("wabbit_alpha.png");
    template <class T> 
    T* pload(const char* arg) {
        T* item = new T((char*)arg);
        resources.push_back(item);
        return item;
        
    }

    //instantiates and pushes resource to manager, returning a pointer for that resource
    //example usage:
    //ImageResource* img = rema.pload<ImageResource> ("wabbit_alpha.png");
    //Texture2DResource* tex = rema.pload<Texture2DResource, Image> ((*img).image);
    template<class T, typename Y>
    T* pload(Y arg) {
        T* item = new T(arg);
        resources.push_back(item);
        return item;
    }

    void push(Resource* res) {
        resources.push_back(res);
    }

    // delete doesn't work, need a template to call the class specifically
    // OR I CAN USE A VIRTUAL FUNCTION. this seems like the best idea

    void clearAll() {
        for(Resource* res : resources) {
            res->unload();
            delete res; //
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