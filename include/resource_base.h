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
    char* path;
public:
    res_type getType() {
        return type;
    }

    virtual void unload() {
        clog(D_RESOURCE, "Attempting to unload a base Resource class!");
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
