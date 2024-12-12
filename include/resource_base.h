#pragma once

#include "debug_stuff.h"
#include <iostream>
#include "raylib.h"
#include <memory>

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



class Resource { // the beginning of it all muahahaha
protected:
    res_type type;

    

    
    
public:
    char* path;
    char* id;

    // if you have to call this, don't
    res_type getType() {
        return type;
    }

    virtual void unload() {
        clog(D_RESOURCE, "Attempting to unload a base Resource class!");
    }

    virtual ~Resource() {};
    
};

class ImageResource: public Resource {
private:
    Image* pointer;
    Image resource;
public:
    ImageResource(char* respath) {
        path = (char*)respath;
        id = respath;
        type = IMAGE;
        resource = LoadImage(path);
        pointer = &resource;
    }

    void unload() {
        UnloadImage(resource);
    }

    Image get() {
        return resource;
    }

    ~ImageResource() {
        unload();
    }


};

class TextureResource: public Resource {
private:
    Texture resource;
    Texture* pointer;
    bool imgPath = false;
    std::shared_ptr<ImageResource> imageRes;
public:
    void deleter(Texture* t) {
        UnloadTexture(*t);
    }

    //Overloaded constructor
    //the first definition using char* will first load an image, then load a texture from that image
    //the second definition will load a texture from a given Image object
    //the second mode of operation requires the image to still be around when loading and unloading
    TextureResource(const char* respath) {
        path = (char*)respath;
        type = TEXTURE2D;
        resource = LoadTexture(respath);
        pointer = &resource;
    }

    TextureResource(std::shared_ptr<ImageResource> i) {
        type = TEXTURE2D;
        imageRes = i;
        resource = LoadTextureFromImage(i.get()->get());
        pointer = &resource;
    }

    Texture get() {
        return resource;
    }

    void unload() {
        UnloadTexture(resource);
    }

    ~TextureResource() {
        unload();
    }
};




Texture errorTexture;

Texture getFromResource(std::shared_ptr<TextureResource> p) {
    if(p.get()) {
        Texture item = p.get()->get();
        return (item);
    }
    return errorTexture;
}



template<typename T, class Y>
T getFromResource(std::shared_ptr<Y> p) {
    if(p.get()) {
        T item = p.get()->get();
        return (item);
    }
    return T();
}

