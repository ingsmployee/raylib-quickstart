/*
ORIGINAL TEXT FROM SOURCE:

Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

For a C++ project simply rename the file to .cpp and re-run the build script 

-- Copyright (c) 2020-2024 Jeffery Myers
--
--This software is provided "as-is", without any express or implied warranty. In no event 
--will the authors be held liable for any damages arising from the use of this software.

--Permission is granted to anyone to use this software for any purpose, including commercial 
--applications, and to alter it and redistribute it freely, subject to the following restrictions:

--  1. The origin of this software must not be misrepresented; you must not claim that you 
--  wrote the original software. If you use this software in a product, an acknowledgment 
--  in the product documentation would be appreciated but is not required.
--
--  2. Altered source versions must be plainly marked as such, and must not be misrepresented
--  as being the original software.
--
--  3. This notice may not be removed or altered from any source distribution.

*/

// I, @ingsoc on Discord and https://steamcommunity.com/id/fdslajkfklsadfjkslfjsd/ (Robot Employee\#1) on Steam, have modified this file/project/repo beyond the original source.

#include <iostream>
#include "raylib.h"
#include <vector>

#include "overall.h"
#include <memory>

int main ()
{
	ChangeDirectory("resources"); //make sure these glorpaglogangaigrangers load properly ykyk

	InitWindow(500, 500, "grano");

	errorTexture = LoadTexture("error.png");

	Game game;

	ResourceManager rema;
	




	//TextureResource* pTexture = (TextureResource*)rema.push(new TextureResource("wabbit_alpha.png"));

	//TextureResource* pTexture = remaPush<TextureResource> (rema, "wabbit_alpha.png");

	//well this seems pretty straightforward to me. probably not the most performant but who cares

	//new version using shared_ptr
	std::shared_ptr<TextureResource> pTexture;

	//TODO:
	/*
	Implement the custom deleters using std::allocate_shared with an Allocator object.
	Have ResourceManager get the std::shared_ptr of a resource which we have loaded from before...

	What it needs to be able to do:
	1. be sorted for ease of looking things up. makes things take longer to load the first time.
	2. be able to be indexed by the resource that was used to make the thing

	Perhaps it could work like this...
	OH MY GOD JUST USE AN ID SYSTEM!!!

	In ResourceManager, when a Resource is requested to be created (using pload());
	1.  Mandate that a const char* identifier is provided, preferably as the first argument
	2.  Check if ResourceManager::resource (which is a std::map<char*, std::shared_ptr<Resource>>)
		already has that char* identifier as a key
	3a. If that char* identifier is already in the map, then return a std::shared_ptr<T>, where T
		is a derived class of Resource. T because pload() is a template function.
	3b. If that char* identifier is NOT in the map, then create a std::shared_ptr<T> and push it
		to the map with that given identifier.

	When a Resource is actually created in ResourceManager, provide it with a pointer back to the
	ResourceManager. This will allow it to tell the ResourceManager when it is unloading, so that
	the ResourceManager can check the std::shared_ptr reference count and reset it if the count
	is 1.


	class ResourceManager;
	
	class Resource {
	protected:
		char* id;
		(Stuff here...)
	public:
		ResourceManager* boundResourceManager;
		Resource(int i) {
			id = i;
		}
	(Rest of the class here...)
	}

	class ImageResource : public Resource {
	(...)
	ImageResource(const char* identifier, const char* path) : Resource(id) {
	}
	(...)
	}

	const char* id
	
	

	ResourceManager::resource is a std::map<char*, std::shared_ptr<Resource>>
	


	Reminder of where I left off / TODO:
	I was working in resource_base.h on ImageResource, trying to figure out how to make a custom allocator
	so that I could have a custom deleter using std::allocate_shared

	just found this link https://stackoverflow.com/a/70630630 but now i have to get off, it's also almost 4AM
	
	
	
	*/
		


	SetTargetFPS(30); //TODO: make function thingy that tells the game to update outside of frame stuff
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);

		game.draw();
		DrawTexture(errorTexture, 150, 150, WHITE);
		DrawTexture(getFromResource<Texture>(pTexture), 30, 30, WHITE);

		EndDrawing();

		if (IsKeyPressed(KEY_G)) {
			pTexture = rema.pload<TextureResource> ("wabbit_alpha.png");
		}
		if (IsKeyPressed(KEY_H)) {
			rema.clearAll();
		}
		
	}
	CloseWindow();
	return 0;
}
