#include <iostream>
#include <raylib.h>
#include "Pong.h"

int player_score = 0;
int cpu_score = 0; 

class Ball {
private: // private variables 

public:// public variables 

	float x, y;
	int speed_x, speed_y;
	int radius;
	int speed_choices[2] = { -1,1 };

private: // private methods

public: // public methods

	void Draw() {
		DrawCircle(x, y, radius, WHITE);
	}

	void Update() {
		x += speed_x;
		y += speed_y;

		if (y + radius >= GetScreenHeight() || y - radius <= 0) //bouncing off walls
		{
			speed_y *= -1;
		}

		if (x + radius >= GetScreenWidth()) //cpu wins
		{
			cpu_score++;
			Reset();
		}
		
		if (x - radius <= 0 ) // player wins
		{
			player_score++;
			Reset();
		}
	}

	void Reset() 
	{
		x = GetScreenWidth() / 2;
		y = GetScreenHeight() / 2;

		speed_x *= speed_choices[GetRandomValue(0, 1)];
		speed_y *= speed_choices[GetRandomValue(0, 1)];
	}

	Ball(float X, float Y, int SPEED_X, int SPEED_Y, int RADIUS) {
		x = X;
		y = Y;
		speed_x = SPEED_X;
		speed_y = SPEED_Y;
		radius = RADIUS;
	}
};


class Paddle {
public:

	float x, y;
	int width, height;
	int speed_y;

protected:
	void LimitMovement() {
		if (y <= 0)
		{
			y = 0;
		}

		if (y + height >= GetScreenHeight())
		{
			y = GetScreenHeight() - height;
		}
	}
public:

	void Draw() {
		DrawRectangle(x,y,width,height, WHITE);
	}

	void Update() 
	{
		if (IsKeyDown(KEY_UP))
		{
			y -= speed_y;
		}
		if (IsKeyDown(KEY_DOWN))
		{
			y += speed_y;
		}
		LimitMovement();
	}

	Paddle(){}

	Paddle(float X, float Y, int WIDTH, int HEIGHT,int SPEED_Y) {

		width = WIDTH;
		height = HEIGHT;
		speed_y = SPEED_Y;
		x = X;
		y = Y;

	}
};

class CpuPaddle : public Paddle {

public:
	
	void Update(float ball_y) 
	{
		if (y + height / 2 > ball_y)
		{
			y -= speed_y;
		}
		
		if (y + height / 2 <= ball_y)
		{
			y += speed_y;
		}
		LimitMovement();
	}
	
	CpuPaddle(float X, float Y, int WIDTH, int HEIGHT, int SPEED_Y) {

		width = WIDTH;
		height = HEIGHT;
		speed_y = SPEED_Y;
		x = X;
		y = Y;
	}
};


int main()
{
	const int screen_width = 1280; // variables for window size 
	const int screen_height = 800;
	
	const int ball_radius = 10;
	const float ball_speed_x = 5;
	const float ball_speed_y = 5;
	const int paddle_width = 20;
	const int paddle_height = 100;
	const int paddle_offset = 10;
	const int paddle_speed = 6;


	Ball ball(screen_width / 2, screen_height / 2, ball_speed_x, ball_speed_y, ball_radius);
	Paddle player(screen_width - paddle_offset - paddle_width,screen_height/2 - paddle_height/2,paddle_width,paddle_height,paddle_speed);
	CpuPaddle cpu(paddle_offset, screen_height / 2 - paddle_height / 2, paddle_width, paddle_height, paddle_speed);

	InitWindow(screen_width, screen_height, "Pong");
	SetTargetFPS(60); // game to run at 60 FPS

	// game loop
	while (WindowShouldClose() == false) 
	{
		BeginDrawing();	
		
		//Updating
		ball.Update();
		player.Update();
		cpu.Update(ball.y);
		
		//Check for collision
		if (CheckCollisionCircleRec(Vector2{ ball.x,ball.y }, ball.radius, Rectangle{ player.x,player.y,float(player.width),float(player.height) }))
		{
			ball.speed_x *= -1;
		}

		if (CheckCollisionCircleRec(Vector2{ ball.x,ball.y }, ball.radius, Rectangle{ cpu.x,cpu.y,float(cpu.width),float(cpu.height) }))
		{
			ball.speed_x *= -1;
		}

		//Drawing game objects
		ClearBackground(BLACK);
		DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
		ball.Draw();
		player.Draw();
		cpu.Draw();
		DrawText(TextFormat("%i",cpu_score), screen_width / 4 -20, 20, 80, WHITE);
		DrawText(TextFormat("%i", player_score), 3*screen_width / 4 -20, 20, 80, WHITE);

		//DrawFPS(10, 10); //displays FPS 
		EndDrawing();
	}

	CloseWindow();
}