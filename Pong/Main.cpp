#include <iostream>
#include <raylib.h>
#include <cmath>

// setting initial scores 
int player_score = 0;
int cpu_score = 0; 

Color Purple = Color{ 145, 70, 255,255 };
Color Dark_Purple = Color{ 135, 31, 235,255 };
Color Light_Purple = Color{ 180, 150, 235,255 };


class Ball {

public:// public variables 

	float x, y; // ball position
	float speed_x, speed_y; //ball speeds
	int radius; //size of ball
	int speed_choices[2] = { -1,1 }; //array for random initial direction
	float init_speed_x;
	float init_speed_y;
	int rotation;
	int rotation_speed;

public: // public methods

	void Draw(Texture texture,bool pog_on_off) 
	{
		Vector2 pos = { x-25,y-25};
		if (pog_on_off)
		{
			rotation += rotation_speed;
			DrawTextureEx(texture, pos,rotation,1, RAYWHITE);
			//DrawCircle(x, y, radius, WHITE);//test image location relative to hitbox
		}
		else 
		{
			DrawCircle(x, y, radius, WHITE);//draws ball to window
		}
	}

	void Update(Sound score) { // updates balls position based on speed, takes sound input 
		x += speed_x;
		y += speed_y;

		if (y + radius >= GetScreenHeight() || y - radius <= 0) //bouncing off walls
		{
			speed_y *= -1;
		}

		if (x + radius >= GetScreenWidth()) //cpu wins
		{
			cpu_score++;
			PlaySound(score);
			Reset();
		}
		
		if (x - radius <= 0 ) // player wins
		{
			player_score++;
			PlaySound(score);
			Reset();
		}
	}

	void Reset() //ball resets to centre of screen, random direction of travel
	{
		x = GetScreenWidth() / 2;
		y = GetScreenHeight() / 2;

		speed_x = init_speed_x;
		speed_y = init_speed_y;

		speed_x *= speed_choices[GetRandomValue(0, 1)];
		speed_y *= speed_choices[GetRandomValue(0, 1)];
	}

	Ball(float X, float Y, int SPEED_X, int SPEED_Y, int RADIUS) { // constructor for ball class
		x = X;
		y = Y;
		speed_x = SPEED_X;
		speed_y = SPEED_Y;
		init_speed_x = SPEED_X;
		init_speed_y = SPEED_Y;
		radius = RADIUS;
		rotation = 0;
		rotation_speed = 0; // rotation off
	}
};


class Paddle { //player paddle
public:

	float x, y; // position
	int width, height; //size of paddle
	int speed_y; //speed of paddle

protected: 
	void LimitMovement() // stops paddle going beyond the screen
	{ 
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

	void Draw() // draws paddle to window
	{
		DrawRectangleRounded(Rectangle{x,y,float(width),float(height)},0.6,10,WHITE);
	}

	void Update() // updates paddles new position 
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

	Paddle(){} // needed for CPU subclass 

	Paddle(float X, float Y, int WIDTH, int HEIGHT,int SPEED_Y) { //constructor 

		width = WIDTH;
		height = HEIGHT;
		speed_y = SPEED_Y;
		x = X;
		y = Y;

	}
};

class CpuPaddle : public Paddle { // subclass CPU paddle

public:
	
	void Update(float ball_y)  // AI position controls 
	{
		if (y + height / 2 > ball_y) // currently tracks ball y position
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
	// variables for window size 
	const int screen_width = 1280; 
	const int screen_height = 800;
	
	//parameters of game 
	const int ball_radius = 10;
	const float ball_speed_x = 10.0f;
	const float ball_speed_y = 9.0f;
	const int paddle_width = 20;
	const int paddle_height = 120;
	const int paddle_offset = 10;
	const int paddle_speed = 8;
	const int paddle_speed_cpu = 8;

	//constructing game objects
	Ball ball(screen_width / 2, screen_height / 2, ball_speed_x, ball_speed_y, ball_radius);
	Paddle player(screen_width - paddle_offset - paddle_width,screen_height/2 - paddle_height/2,paddle_width,paddle_height,paddle_speed);
	CpuPaddle cpu(paddle_offset, screen_height / 2 - paddle_height / 2, paddle_width, paddle_height, paddle_speed_cpu);

	//creating window
	InitWindow(screen_width, screen_height, "Pong");
	SetTargetFPS(60); // game to run at 60 FPS

	//randomises ball starting direction
	ball.Reset();

	Image Poggers = LoadImage("Poggers.png");
	ImageResize(&Poggers, 50, 50);
	Texture2D texture = LoadTextureFromImage(Poggers);

	
	

	InitAudioDevice();      // Initialize audio device

	Sound fx_Pong = LoadSound("pong.wav"); // loads sound
	Sound fx_Score = LoadSound("score.wav");

	// game loop
	while (WindowShouldClose() == false) 
	{
		BeginDrawing();	
		
		//Updating positions
		ball.Update(fx_Score);
		player.Update();
		cpu.Update(ball.y);
		
		//Check for collision ball and player
		if (CheckCollisionCircleRec(Vector2{ ball.x,ball.y }, ball.radius, Rectangle{ player.x,player.y,float(player.width),float(player.height) }))
		{
			ball.speed_x *= -1.1f;
			PlaySound(fx_Pong);
		}

		//Check for collision ball and cpu
		if (CheckCollisionCircleRec(Vector2{ ball.x,ball.y }, ball.radius, Rectangle{ cpu.x,cpu.y,float(cpu.width),float(cpu.height) }))
		{
			ball.speed_x *= -1.1f;
			PlaySound(fx_Pong);

		}
		
		

		//Drawing game objects
		ClearBackground(Dark_Purple);
		DrawRectangle(screen_width/2,0,screen_width/2,screen_height,Purple);
		DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Purple);
		DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
		ball.Draw(texture,1);
		player.Draw();
		cpu.Draw();
		DrawText(TextFormat("%i",cpu_score), screen_width / 4 -20, 20, 80, WHITE);
		DrawText(TextFormat("%i", player_score), 3*screen_width / 4 -20, 20, 80, WHITE);

		//DrawFPS(10, 10); //displays FPS 
		EndDrawing();
	}

	CloseWindow();
}