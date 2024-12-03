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

	std::vector<Resource> g;

	InitWindow(500, 500, "grano");

	Game game;

	std::cout << typeid(Game).name();
	std::cout << typeid(game).name();

	ImageResource ir = *new ImageResource("wabbit_alpha.png");
	Texture2DResource tr = *new Texture2DResource("wabbit_alpha.png");

	SetTargetFPS(30); //TODO: make function thingy that tells the game to update outside of frame stuff
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);

		game.draw();

		DrawTexture(tr.texture, 30, 30, WHITE);

		EndDrawing();

		if (IsKeyDown(KEY_G)) {
			ir.load();
		}
		if (IsKeyDown(KEY_H)) {
			ir.unload();
		}
		
	}
	CloseWindow();
	return 0;
}
