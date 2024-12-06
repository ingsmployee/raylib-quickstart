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

template<typename T, class Y>
T getFromResource(std::shared_ptr<Y> p) {
    return (p.get()->get());
}

class Resource { // the beginning of it all muahahaha
protected:
    res_type type;

public:
    char* path;

    // if you have to call this, something is wrong
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
    std::shared_ptr<Image> pointer;
public:
    void deleter(Image* i) {
        UnloadImage(*i);
    }

    ImageResource(const char* respath) {
        path = (char*)respath;
        type = IMAGE;
        load();
    }

    std::shared_ptr<Image> load() {
        if (!pointer) {
            pointer = std::make_shared<Image> (LoadImage(path), deleter);
        }
        return pointer;
    }

    void unload() {
        pointer.reset();
    }

    Image get() {
        return *(pointer.get());
    }

    ~ImageResource() {
        unload();
    }


};

class Texture2DResource: public Resource {
private:
    std::shared_ptr<Texture> pointer;
    bool imgPath = false;
    std::shared_ptr<ImageResource> imageResPointer;
public:
    void deleter(Texture* t) {
        UnloadTexture(*t);
    }

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

    Texture2DResource(ImageResource fromImageResource) {
        type = TEXTURE2D;
        imageResPointer = std::make_shared<ImageResource>(fromImageResource);
        load();
    }

    Texture get() {
        return *(pointer.get());
    }

    std::shared_ptr<Texture> load() {
        if (!pointer) {
            if (imageResPointer) {
                Image temp = LoadImage(path); // very temporary
                pointer = std::make_shared<Texture> (LoadTextureFromImage(temp), deleter);
                UnloadImage(temp);
            }
            else {
                //note that this dereferences the get() from i_pointer, not i_pointer itself
                pointer = std::make_shared<Texture> (LoadTextureFromImage(getFromResource<Image>(imageResPointer)), deleter);
            }
        }
        return pointer;
    }

    void unload() {
        pointer.reset();
    }

    ~Texture2DResource() {
        unload();
    }
};
