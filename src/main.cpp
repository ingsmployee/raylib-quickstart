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

	MemoryManager grongle = MemoryManager();
	grongle.showRAM();

	Image wabbit = LoadImage("wabbit_alpha.png");
	grongle.registerResource(&wabbit, IMAGE);
	grongle.showRAM();

	Texture2D wabbit_texture = LoadTextureFromImage(wabbit);
	grongle.registerResource(&wabbit_texture, TEXTURE);
	grongle.unloadResource(&wabbit, IMAGE);
	grongle.showRAM();
	grongle.showVRAM();

	SetTargetFPS(30);
	while (!WindowShouldClose()) {
		BeginDrawing();
		DrawTexture(wabbit_texture, 50, 50, WHITE);
		EndDrawing();

		if (IsKeyDown(KEY_A)) {
			grongle.unloadResource(&wabbit_texture, TEXTURE);
			grongle.showVRAM();
		}

		
	}
	CloseWindow();
	return 0;
}
