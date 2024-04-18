#include <iostream>
#include <raylib.h>
#include "Pong.h"

class Ball {
private: // private variables 

public:// public variables 

	float x, y;
	int speed_x, speed_y;
	int radius;

private: // private methods

public: // public methods

	void Draw() {
		DrawCircle(x, y, radius, WHITE);
	}

	void Update() {
		x += speed_x;
		y += speed_y;
	}

	Ball(float X, float Y, int SPEED_X, int SPEED_Y) {
		x = X;
		y = Y;
		speed_x = SPEED_X;
		speed_y = SPEED_Y;
		radius = 10;
	}
};

int main()
{
	const int screen_width = 1280; // variables for window size 
	const int screen_height = 800;
	
	const int ball_radius = 10;
	const int paddle_width = 20;
	const int paddle_height = 100;
	const int paddle_offset = 10;

	Ball ball(screen_width / 2,screen_height/2,5,5);

	InitWindow(screen_width, screen_height, "Pong");
	SetTargetFPS(60); // game to run at 60 FPS

	// game loop
	while (WindowShouldClose() == false) 
	{
		BeginDrawing();	
		
		//Updating
		ball.Update();


		//Drawing game objects
		ClearBackground(BLACK);
		DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
		ball.Draw();
		DrawRectangle(paddle_offset, screen_height / 2 - paddle_height / 2, paddle_width, paddle_height, WHITE);
		DrawRectangle(screen_width - paddle_width - paddle_offset, screen_height / 2 - paddle_height / 2, paddle_width, paddle_height, WHITE);
		


		//DrawFPS(10, 10); //displays FPS 
		EndDrawing();
	}

	CloseWindow();
}