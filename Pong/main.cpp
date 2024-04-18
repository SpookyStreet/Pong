#include <iostream>
#include <raylib.h>

int main()
{
	InitWindow(800, 600, "Pong");

	while (!WindowShouldClose())
	{
		BeginDrawing();
			ClearBackground(BLACK);
		EndDrawing();
	}

	CloseWindow();
}