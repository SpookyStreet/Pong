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
	float speed_y_bounce[2] = { 1,1.1 };
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

class Paddle_Player_2 : public Paddle { // subclass for player 2

public:

	void Update() // updates paddles new position 
	{
		if (IsKeyDown(KEY_W))
		{
			y -= speed_y;
		}
		if (IsKeyDown(KEY_S))
		{
			y += speed_y;
		}
		LimitMovement();
	}

	Paddle_Player_2(float X, float Y, int WIDTH, int HEIGHT, int SPEED_Y) {

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
	Paddle_Player_2 player_2(paddle_offset, screen_height / 2 - paddle_height / 2, paddle_width, paddle_height, paddle_speed);

	//creating window
	InitWindow(screen_width, screen_height, "Pong");
	SetTargetFPS(60); // game to run at 60 FPS

	//randomises ball starting direction
	ball.Reset();

	Image Poggers = LoadImage("Poggers.png");
	ImageResize(&Poggers, 50, 50);
	Texture2D texture = LoadTextureFromImage(Poggers);

	Image Poggers_menu = LoadImage("Poggers.png");
	ImageResize(&Poggers_menu, 300, 300);
	Texture2D texture_menu = LoadTextureFromImage(Poggers_menu);

	InitAudioDevice();      // Initialize audio device

	Sound fx_Pong = LoadSound("pong.wav"); // loads sound
	Sound fx_Score = LoadSound("score.wav");
	Sound fx_Button = LoadSound("button.wav");
	Sound fx_Pog = LoadSound("pog.wav");

	//menu
	bool isInMenu = true;
	
	int button_height = 100;
	int button_width = 400;
	int play_state = 0; // 0 normal, 1 hover
	bool play_action = false;
	int pog_state = 0; // 0 normal, 1 hover
	bool pog_action = false;
	int reset_state = 0; // 0 normal, 1 hover
	bool reset_action = false;
	int multiplayer_state = 0; // 0 normal, 1 hover
	bool multiplayer_action = false;


	bool pog_on = false;
	
	bool multiplayer_off = true;

	Rectangle play_bounds = { (screen_width - button_width) / 2, (screen_height - button_height) / 3, button_width, button_height };
	Rectangle pog_bounds = { (screen_width - button_width) / 2, (screen_height - button_height) / 2, button_width, button_height };
	Rectangle reset_bounds = { (screen_width - button_width) / 2, 2 * (screen_height - button_height) / 3, button_width, button_height };
	Rectangle multiplayer_bounds = { (screen_width - button_width) / 2, 5 * (screen_height - button_height) / 6, button_width, button_height };

	Vector2  mousePoint = { 0.0f,0.0f };

	// game loop
	while (WindowShouldClose() == false) 
	{
		
		mousePoint = GetMousePosition();
		bool play_action = false;
		bool pog_action = false;
		bool reset_action = false;
		bool multiplayer_action = false;

		//play button
		if (CheckCollisionPointRec(mousePoint, play_bounds))
		{
			play_state = 1;

			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
				play_action = true;
			}
		}
		else
		{
			play_state = 0;
		}

		if (play_action) 
		{
			PlaySound(fx_Button);
			isInMenu = false;
		}

		// poggers button
		if (CheckCollisionPointRec(mousePoint, pog_bounds))
		{
			pog_state = 1;

			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) 
			{
				pog_action = true;
				
			}
		}
		else
		{
			pog_state = 0;
		}

		if (pog_action)
		{
			if (pog_on) {
				pog_on = false;
			}
			else{
				PlaySound(fx_Pog);
				pog_on = true;
			}
		}

		//reset button
		if (CheckCollisionPointRec(mousePoint, reset_bounds))
		{
			reset_state = 1;

			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
				reset_action = true;

			}
		}
		else
		{
			reset_state = 0;
		}

		if (reset_action)
		{
			PlaySound(fx_Button);
			player_score = 0;
			cpu_score = 0;
			ball.Reset();
			pog_on = false;
		}
		
		// multiplayer button
		if (CheckCollisionPointRec(mousePoint, multiplayer_bounds))
		{
			multiplayer_state = 1;

			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
			{
				multiplayer_action = true;

			}
		}
		else
		{
			multiplayer_state = 0;
		}

		if (multiplayer_action)
		{
			player_score = 0;
			cpu_score = 0;
			ball.Reset();
			if (multiplayer_off) {
				multiplayer_off = false;
				PlaySound(fx_Pog);
			}
			else {
				PlaySound(fx_Button);
				multiplayer_off = true;
			}
		}

		//menu return
		if (isInMenu)
		{
			if (IsKeyPressed(KEY_SPACE)) {
				isInMenu = false;
			}
		}
		else
		{
			if (IsKeyPressed(KEY_SPACE)) {
				isInMenu = true;
			}
		}




		BeginDrawing();	
		
		if (isInMenu) 
		{
			ClearBackground(Dark_Purple);
			DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Purple);
			DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Purple);
			DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
			DrawText("Ponggers Brother!!", (screen_width- MeasureText("Ponggers Brother!!", 80))/2 ,20, 80, WHITE);
			DrawText("space to toggle menu", 20,screen_height - 40, 20, WHITE);
			DrawText("Player 1 keys: up & down", screen_width - 300, screen_height - 80, 20, WHITE);
			DrawText("Player 2 keys: w & s", screen_width - 300, screen_height - 40, 20, WHITE);
			
			
			if (play_state)
			{
				DrawRectangle((screen_width - button_width) / 2, (screen_height - button_height) / 3, button_width, button_height, BLACK);
				DrawText("Play!", (screen_width - MeasureText("Play!", 80)) / 2, (screen_height - button_height) / 3 + 10, 80, Purple);
			}
			else
			{
				DrawRectangle((screen_width - button_width) / 2, (screen_height - button_height) / 3, button_width, button_height, WHITE);
				DrawText("Play!", (screen_width - MeasureText("Play!", 80)) / 2, (screen_height - button_height) / 3 + 10, 80, Purple);
			}

			if (pog_state)
			{
				DrawRectangle((screen_width - button_width) / 2, (screen_height - button_height) / 2, button_width, button_height, BLACK);
				DrawText("Poggers!", (screen_width - MeasureText("Poggers!", 80)) / 2, (screen_height - button_height) / 2 + 10, 80, Purple);
			}
			else 
			{
				DrawRectangle((screen_width - button_width) / 2, (screen_height - button_height) / 2, button_width, button_height, WHITE);
				DrawText("Poggers!", (screen_width - MeasureText("Poggers!", 80)) / 2, (screen_height - button_height) / 2 + 10, 80, Purple);
			}

			if (reset_state)
			{
				DrawRectangle((screen_width - button_width) / 2, 2 * (screen_height - button_height) / 3, button_width, button_height, BLACK);
				DrawText("Reset", (screen_width - MeasureText("Reset", 80)) / 2, 2 * (screen_height - button_height) / 3 + 10, 80, Purple);
			}
			else
			{
				DrawRectangle((screen_width - button_width) / 2, 2 * (screen_height - button_height) / 3, button_width, button_height, WHITE);
				DrawText("Reset", (screen_width - MeasureText("Reset", 80)) / 2, 2 * (screen_height - button_height) / 3 + 10, 80, Purple);
			}

			if (multiplayer_off) {
				if (multiplayer_state)
				{
					DrawRectangle((screen_width - button_width) / 2, 5 * (screen_height - button_height) / 6, button_width, button_height, BLACK);
					DrawText("2 Player", (screen_width - MeasureText("2 Player", 80)) / 2, 5 * (screen_height - button_height) / 6 + 10, 80, Purple);
				}
				else
				{
					DrawRectangle((screen_width - button_width) / 2, 5 * (screen_height - button_height) / 6, button_width, button_height, WHITE);
					DrawText("2 Player", (screen_width - MeasureText("2 Player", 80)) / 2, 5 * (screen_height - button_height) / 6 + 10, 80, Purple);
				}
			}
			else {
				if (multiplayer_state)
				{
					DrawRectangle((screen_width - button_width) / 2, 5 * (screen_height - button_height) / 6, button_width, button_height, BLACK);
					DrawText("CPU", (screen_width - MeasureText("CPU", 80)) / 2, 5 * (screen_height - button_height) / 6 + 10, 80, Purple);
				}
				else
				{
					DrawRectangle((screen_width - button_width) / 2, 5 * (screen_height - button_height) / 6, button_width, button_height, WHITE);
					DrawText("CPU", (screen_width - MeasureText("CPU", 80)) / 2, 5 * (screen_height - button_height) / 6 + 10, 80, Purple);
				}
			}


			if (pog_on) {

				DrawTexture(texture_menu, 5 * screen_width / 6 - 150, screen_height / 2 - 150, WHITE);
				if (!multiplayer_off) {
					DrawTexture(texture_menu, screen_width / 6 - 150, screen_height / 2 - 150, WHITE);
				}
			}


		}
		else
		{
			if (multiplayer_off)
			{


				//Updating positions
				ball.Update(fx_Score);
				player.Update();
				cpu.Update(ball.y);

				//Check for collision ball and player
				if (CheckCollisionCircleRec(Vector2{ ball.x,ball.y }, ball.radius, Rectangle{ player.x,player.y,float(player.width),float(player.height) }))
				{
					ball.speed_x *= -1.1f;
					ball.speed_y *= ball.speed_y_bounce[GetRandomValue(0, 1)];
					PlaySound(fx_Pong);
				}

				//Check for collision ball and cpu
				if (CheckCollisionCircleRec(Vector2{ ball.x,ball.y }, ball.radius, Rectangle{ cpu.x,cpu.y,float(cpu.width),float(cpu.height) }))
				{
					ball.speed_x *= -1.1f;
					//ball.speed_y *= ball.speed_y_bounce[GetRandomValue(0, 1)];
					PlaySound(fx_Pong);

				}



				//Drawing game objects
				ClearBackground(Dark_Purple);
				DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Purple);
				DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Purple);
				DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
				ball.Draw(texture, pog_on);
				player.Draw();
				cpu.Draw();
				DrawText(TextFormat("%i", cpu_score), screen_width / 4 - 20, 20, 80, WHITE);
				DrawText(TextFormat("%i", player_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);
			}
			else
			{
				//Updating positions
				ball.Update(fx_Score);
				player.Update();
				player_2.Update();

				//Check for collision ball and player
				if (CheckCollisionCircleRec(Vector2{ ball.x,ball.y }, ball.radius, Rectangle{ player.x,player.y,float(player.width),float(player.height) }))
				{
					ball.speed_x *= -1.1f;
					ball.speed_y *= ball.speed_y_bounce[GetRandomValue(0, 1)];
					PlaySound(fx_Pong);
				}

				//Check for collision ball and player_2
				if (CheckCollisionCircleRec(Vector2{ ball.x,ball.y }, ball.radius, Rectangle{ player_2.x,player_2.y,float(player_2.width),float(player_2.height) }))
				{
					ball.speed_x *= -1.1f;
					ball.speed_y *= ball.speed_y_bounce[GetRandomValue(0, 1)];
					PlaySound(fx_Pong);

				}



				//Drawing game objects
				ClearBackground(Dark_Purple);
				DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Purple);
				DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Purple);
				DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
				ball.Draw(texture, pog_on);
				player.Draw();
				player_2.Draw();
				DrawText(TextFormat("%i", cpu_score), screen_width / 4 - 20, 20, 80, WHITE);
				DrawText(TextFormat("%i", player_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);
			}
		}
		//DrawFPS(10, 10); //displays FPS 
		EndDrawing();
	}

	CloseWindow();
}