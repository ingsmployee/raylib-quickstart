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


#include "raylib.h"

#include "overall.h"


int main ()
{
	ChangeDirectory("resources"); //make sure these glorpaglogangaigrangers load properly ykyk

	

	InitWindow(500, 500, "grano");

	Game game;

	ResourceManager rema;
	




		//Texture2DResource* pTexture = (Texture2DResource*)rema.push(new Texture2DResource("wabbit_alpha.png"));
		//Image i = LoadImage("wabbit_alpha.png");
		//Texture2DResource* pTexture = remaPush<Texture2DResource> (rema, "wabbit_alpha.png");

		//well this seems pretty straightforward to me. probably not the most performant but who cares
		Texture2DResource* pTexture = rema.pload<Texture2DResource> ("wabbit_alpha.png");

		
		std::cout << pTexture << std::endl;

		//trying to get this syntax to work using overloaded or newly defined operators
		//rema load new Texture2DResource("wabbit_alpha.png");
		


	SetTargetFPS(30); //TODO: make function thingy that tells the game to update outside of frame stuff
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);

		game.draw();

		DrawTexture((*pTexture).texture, 30, 30, WHITE);

		EndDrawing();

		if (IsKeyDown(KEY_G)) {
			pTexture = rema.pload<Texture2DResource> ("wabbit_alpha.png");
		}
		if (IsKeyDown(KEY_H)) {
			rema.clearAll();
		}
		
	}
	CloseWindow();
	return 0;
}
