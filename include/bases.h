#pragma once

#include "debug_stuff.h"
#include <iostream>
#include "raylib.h"
#include <memory>
#include "prototypes.h"

Texture errorTexture; //note that this is a raylib Texture, not a TextureResource.



//organized for clarity. doesn't really matter
enum res_type { 
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

enum obj_message {
    MESSAGE_A

};

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

//resource class definitions
class ImageResource: public Resource {
private:
    Image* pointer;
    Image resource;
public:
    ImageResource(const char* respath) {
        path = (char*)respath;
        id = (char*)respath;
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



class GameObject {
public:
    std::weak_ptr<Level> levelPtr;
    virtual void draw() {}; //so we can customize draw
    virtual void update() {}; //so we can customize update

    virtual void tell(obj_message message, void* args) {};

    virtual ~GameObject() {}; //so we can customize unload deconstructors
};

class ObjectManager {
private:
    //for sorting or something idk lmfao
    //probably unoptimized
    std::map<int, std::shared_ptr<GameObject>> objects;
    //basically just a sorted vector
    //begins at 0


public:
    Level* level;
    std::weak_ptr<ResourceManager> rema;

    

    //looks up the given id. if there's a std::shared_ptr<GameObject> with that id in the manager,
    //will return it. if there is no such thing with that id, it will create one.
    //tldr; will not instantiate objects over and over
    // there is quite literally no reason to use this. TODO: exterminate
    template <class T, class...Y>
    std::weak_ptr<T> summonGameObject(int index, Y...args) {
        if (objects.count(index) == 0) {
            // if there's nothing with that id, make a new one...
            return createGameObject<T>(args...);
        }
        else {
            //if there's something with that id, just return that...
            //cast it to be of type T
            return(objects.at(index));
        }
    }

    //now this is what you want to use for instantiating objects...
    //it will always just make a new object, and return a weak_ptr to that object
    template <class T, class...Y>
    std::weak_ptr<T> createGameObject(Y...args) {
        std::shared_ptr<T> item (new T(args...));
        objects.insert(std::pair<int, std::shared_ptr<T>>(objects.size(), item));
        return item; //returns weak_ptr
    }


    //removes all objects which are only referenced by shared_ptr in the map...
    //aka removes objects which aren't being used for anything
    // TODO: exterminate
    // because std::weak_ptr doesn't contribute to the use count
    // don't use this
    void removeAllDangling() {
        //yes im gonna use evil auto keyword... should be clear that these are iterators for the map
        auto begin = objects.begin();
        auto end = objects.end();

        while(begin != end) {
            if (begin->second.use_count() == 1) {
                objects.erase(begin);
            }
            begin++;
        }
    }

    void clearAll() {
        objects.clear();
    }

    //returns a pointer to the map, to avoid copying
    //if you wanna iterate on each object, maybe getBeginIterator() would be better idk
    std::map<int, std::shared_ptr<GameObject>>* getMap() {
        return &objects;
    }

    //returns a .begin() iterator for the map
    std::map<int, std::shared_ptr<GameObject>>::iterator getBeginIterator() {
        return objects.begin();
    }

    //returns a .end() iterator for the map
    std::map<int, std::shared_ptr<GameObject>>::iterator getEndIterator() {
        return objects.end();
    }



    //TODO: add "unload" thing

    /* Features I need:
    - constructor that sets the starting size of the map
    - "unload all objects"
    - get an iterator for the map
    - perhaps "execute this function on all objects" using lambdas' [] stuff
    - instantiateObject() which works like how it looks it would

    maybe try having the map contain a struct instead of a direct shared_ptr OR 
    (better solution) make GameObject have a type thing
    
    */

};
class ResourceManager {
private:
    //this is designed to be instantiated per-level, maybe idk
    std::map<const char*, std::shared_ptr<Resource>> resources;

public:
    const char* findResource(std::shared_ptr<Resource>& item) {
        for(const auto &m : resources) {
            if (m.first == (*item.get()).id) {
                return m.first;
            }
        }
        return "";
    }

    void removeResource(std::shared_ptr<Resource> res) {
        const char* id = findResource(res);
        if (id != "") {
            resources.erase(id);
        }
        else {
            clog(D_RESOURCE, "Could not find resource in ResourceManager. Are you checking the right one?"); //this is asswater
            //alright future me here. wtf does asswater mean.
        }
    }

    void clearAll() { //probably unoptimized
        resources.clear();
    }


    //looks up the id... if there's a std::shared_ptr<Resource> with that id in this ResourceManager,
    //it will return a std::weak_ptr<T> of that resource.
    // if there's no such std::shared_ptr<Resource>, it will instantiate a new one and return that.

    template <class T, class...Y>
    std::weak_ptr<T> summonResource(const char* id, Y...args) {
        if (resources.count(id) == 0) {
            // if there's nothing with that id, make a new one...
            std::shared_ptr<T> item (new T(id, args...));
            item->id = (char*)id;
            resources.insert(std::pair<const char*, std::shared_ptr<Resource>>(id, std::static_pointer_cast<Resource>(item)));
            return(item);
        }
        else {
            //if there's something with that id, just return that...
            //cast it to be of type T
            return(std::dynamic_pointer_cast<T>(resources.at(id)));
        }
    }




};

class Level {
protected:
    std::shared_ptr<ResourceManager> rema;

    std::shared_ptr<ObjectManager> obma;

    std::string name;
public:

    //adds and object and puts it in the object manager
    //example code: 
    //Centipede* cent = addObject<Centipede> (Vector2{1,1}, Vector2{1,1}, 5);
    template <class T, class... A>     
    std::weak_ptr<T> addObject(A... args) {
        
        std::weak_ptr<T> obj = obma->createGameObject(args...);
        //give the object a pointer to us
        obj->levelPtr = this;
        return obj;
    }
    //note that it does not care about if there's an existing object and will always make another
    //very similar to ResourceManager

    Level() {
        clog(D_MINIMAL, "Making new level!");
        rema = std::make_shared<ResourceManager>();
        obma = std::make_shared<ObjectManager>();
        obma->level = this;
        obma->rema = rema;
    }

    virtual ~Level() {
        clog(D_MINIMAL, "Deleting level!");
    }

    //overwrite these if you want... they just call draw() on each object
    virtual void draw() {drawObjects();}
    virtual void update() {updateObjects();}

    void drawObjects() {
        auto begin = obma->getBeginIterator();
        auto end = obma->getEndIterator();

        while(begin != end) {
            begin->second->draw();
            begin++;
        }
    }

    void updateObjects() {
        auto begin = obma->getBeginIterator();
        auto end = obma->getEndIterator();

        while(begin != end) {
            // at least it's not java
            begin->second->update();
            begin++;
        }
    }

    void addResource(std::shared_ptr<Resource> res) {
        
    }
};


// this class essentially boils down to a level manager.
// separate from level so that I can have things like UI
// be separate "levels" and be overlayed on top of the previous level.
class Game {
private:
    bool shouldQuit;
    std::shared_ptr<Level> currentLevel;

public:
    void requestQuit() {
        shouldQuit = true;
    }

    std::weak_ptr<Level> switchToLevel(std::weak_ptr<Level> target) {
        clog(D_MINIMAL, "yes. yes. yse. yes. yes.");
        currentLevel.reset();
        currentLevel = target.lock();
        return target;
    }

    Level getLevel() {
        return *currentLevel;
    }

    void draw() {
        currentLevel->draw();
    }

    void update() {
        currentLevel->update();
    }
};