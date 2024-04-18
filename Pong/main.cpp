#include <iostream>
#include <raylib.h>

int main()
{
	const int screen_width = 1280; // variables for window size 
	const int screen_height = 800;
	
	const int ball_radius = 10;
	const int paddle_width = 20;
	const int paddle_height = 100;
	const int paddle_offset = 10;


	InitWindow(screen_width, screen_height, "Pong");
	SetTargetFPS(60); // game to run at 60 FPS

	// game loop
	while (WindowShouldClose() == false) 
	{
		BeginDrawing();	
		
		//Drawing game objects
		DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
		DrawCircle(screen_width/2, screen_height/2, ball_radius, WHITE);
		DrawRectangle(paddle_offset, screen_height / 2 - paddle_height / 2, paddle_width, paddle_height, WHITE);
		DrawRectangle(screen_width - paddle_width - paddle_offset, screen_height / 2 - paddle_height / 2, paddle_width, paddle_height, WHITE);
		


		//DrawFPS(10, 10); //displays FPS 
		EndDrawing();
	}

	CloseWindow();
}