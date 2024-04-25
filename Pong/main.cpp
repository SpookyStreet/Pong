#include <iostream>
#include <raylib.h>

// setting initial scores 
int player_score = 0;
int cpu_score = 0; 

// defining colours to be used
Color Purple = Color{ 145, 70, 255,255 };
Color Dark_Purple = Color{ 135, 31, 235,255 };
Color Light_Purple = Color{ 180, 150, 235,255 };
Color Yellow = Color{255,255,0, 255};

// class to store methods and member variables for the ball
class Ball {
public:// public variables 
	float x, y; // ball position
	float speed_x, speed_y; //ball speeds
	int radius; //size of ball
	int speed_choices[2] = { -1,1 }; //array for random initial direction
	float speed_y_bounce[2] = { 1.0f,1.015f }; //on paddle bounce the ball's y speed is multiplied by one of these values at random. used as tool to balance cpu difficulty greater difference in y speed, harder to match
	float speed_x_bounce = -1.1f; // on paddle bounce the ball's x speed is multiplied by this value 
	float init_speed_x;// store initial speed for reset
	float init_speed_y;// store initial speed for reset
	float rotation = 0;//starting rotation of texture
	int rotation_speed;//speed at which the texture can rotate
	int rotation_speed_init;
	float rotation_multiply [6] = { -0.9f,-1.0f,-1.1f,0.9f,1.0f,1.1f };//on collision the rotation speed is randomly multiplied with one of these values

public: // public methods

	void Draw(Texture texture,bool pog_on_off) 
	{
		if (pog_on_off)
		{
			DrawTexturePro(texture, { 0,0,50,50},{x,y,50,50},{25,25}, rotation, RAYWHITE);//Draws texture to window, origin on centre of ball, rotation around origin
			//DrawCircle(x, y, radius, WHITE);//test texture location relative to ball hitbox
		}
		else 
		{
			DrawCircle(x, y, radius, Yellow);//draws ball to window
		}
	}

	void Update(Sound score) { // updates balls position based on speed, takes sound input 
		x += speed_x;
		y += speed_y;
		rotation += rotation_speed; //rotation increases by speed of rotation

		if (y + radius >= GetScreenHeight() || y - radius <= 0) //bouncing off top and bottom walls
		{
			speed_y *= -1;
		}

		if (x + radius >= GetScreenWidth()) //cpu/player_2 wins if ball goes past the right wall 
		{
			cpu_score++; //increase global variable for cpu/player_2 score
			PlaySound(score); //plays the sound for a score
			Reset(); //resets the ball
		}
		
		if (x - radius <= 0 ) // player wins if the ball goes off the left wall
		{
			player_score++;//increase global variable for player score
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

		rotation = 0; // resets rotation values 
		rotation_speed = rotation_speed_init;
	}

	Ball(float X, float Y, int SPEED_X, int SPEED_Y, int RADIUS, int ROTATION_SPEED) { // constructor for ball class
		x = X;
		y = Y;
		speed_x = SPEED_X;
		speed_y = SPEED_Y;
		init_speed_x = SPEED_X;
		init_speed_y = SPEED_Y;
		radius = RADIUS;
		rotation_speed = ROTATION_SPEED; 
		rotation_speed_init = ROTATION_SPEED;
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
		DrawRectangleRounded(Rectangle{x,y,float(width),float(height)},0.6f,10,WHITE);
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

	Paddle(){
		width = 0;
		height = 0;
		speed_y = 0;
		x = 0;
		y = 0;
	} //default constructor needed for CPU & player_2 subclasses

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
		if (y + height / 2 > ball_y) // tracks ball y position
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
	const float screen_width = 1280; //changed to float to avoid errors when compiling for html as is divided by 2 in places
	const float screen_height = 800;
	
	//parameters of game 
	const int ball_radius = 15;
	const float ball_speed_x = 10.0f;
	const float ball_speed_y = 9.0f;
	const float paddle_width = 20;
	const float paddle_height = 120;
	const int paddle_offset = 10;
	const int paddle_speed = 8;
	const int paddle_speed_cpu = 8;
	const int rotation_speed = 10;

	//constructing game objects
	Ball ball(screen_width / 2, screen_height / 2, ball_speed_x, ball_speed_y, ball_radius, rotation_speed);
	Paddle player(screen_width - paddle_offset - paddle_width,screen_height/2 - paddle_height/2,paddle_width,paddle_height,paddle_speed);
	CpuPaddle cpu(paddle_offset, screen_height / 2 - paddle_height / 2, paddle_width, paddle_height, paddle_speed_cpu);
	Paddle_Player_2 player_2(paddle_offset, screen_height / 2 - paddle_height / 2, paddle_width, paddle_height, paddle_speed);

	//creating window
	InitWindow(screen_width, screen_height, "Ponggers Brother!!");
	SetTargetFPS(60); // game to run at 60 FPS

	//randomises ball starting direction
	ball.Reset();

	//loading textures
	Image Poggers = LoadImage("Graphics/panda_poggers.png");
	ImageResize(&Poggers, 50, 50);
	Texture2D texture = LoadTextureFromImage(Poggers);

	Image Poggers_menu = LoadImage("Graphics/panda_poggers.png");
	ImageResize(&Poggers_menu, 300, 300);
	Texture2D texture_menu = LoadTextureFromImage(Poggers_menu);

	SetWindowIcon(Poggers);
	// Initialize audio device
	InitAudioDevice();      

	Sound fx_Pong = LoadSound("Sounds/pong.wav"); 
	Sound fx_Score = LoadSound("Sounds/score.wav");
	Sound fx_Button = LoadSound("Sounds/button.wav");
	Sound fx_Pog = LoadSound("Sounds/pog.wav");

	//Menu on/off
	bool isInMenu = true;

	//varibles for menu buttons
	float button_height = 100;
	float button_width = 400;
	int play_state = 0; // 0 normal, 1 hover
	bool play_action = false;
	int pog_state = 0; // 0 normal, 1 hover
	bool pog_action = false;
	int reset_state = 0; // 0 normal, 1 hover
	bool reset_action = false;
	int multiplayer_state = 0; // 0 normal, 1 hover
	bool multiplayer_action = false;

	Rectangle play_bounds = { (screen_width - button_width) / 2, (screen_height - button_height) / 3, button_width, button_height };
	Rectangle pog_bounds = { (screen_width - button_width) / 2, (screen_height - button_height) / 2, button_width, button_height };
	Rectangle reset_bounds = { (screen_width - button_width) / 2, 2 * (screen_height - button_height) / 3, button_width, button_height };
	Rectangle multiplayer_bounds = { (screen_width - button_width) / 2, 5 * (screen_height - button_height) / 6, button_width, button_height };

	//varible to track mouse location for buttons
	Vector2  mousePoint = { 0.0f,0.0f };

	//poggers state on/off
	bool pog_on = false;

	//multiplayer on/off
	bool multiplayer_off = true;

	// game loop
	while (WindowShouldClose() == false) 
	{
		mousePoint = GetMousePosition(); //gets mouse position 
		//reseting button states	
		bool play_action = false;
		bool pog_action = false;
		bool reset_action = false;
		bool multiplayer_action = false;

		//play button
		if (CheckCollisionPointRec(mousePoint, play_bounds))// is the mouse over the play button
		{
			play_state = 1;//changes look of button

			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {//mouse clicked on button
				play_action = true;//triggers what the button does
			}
		}
		else
		{
			play_state = 0;//mouse not over button
		}

		if (play_action) //activates when play button pressed
		{
			PlaySound(fx_Button);
			isInMenu = false; //switches from menu to game
		}

		// poggers button
		if (CheckCollisionPointRec(mousePoint, pog_bounds))// is the mouse over the poggers button
		{
			pog_state = 1;

			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) 
			{
				pog_action = true;//triggers what the button does
				
			}
		}
		else
		{
			pog_state = 0;
		}

		if (pog_action)//activates when poggers button pressed
		{
			if (pog_on) {//toggles poggers state
				PlaySound(fx_Button);
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

		if (reset_action)//activates when reset button pressed, resets scores, object locations, poggers state
		{
			PlaySound(fx_Button);
			player_score = 0;
			cpu_score = 0;
			ball.Reset();
			pog_on = false;
			player.y = (screen_height - player.height) / 2;
			cpu.y = (screen_height - cpu.height) / 2;
			player_2.y = (screen_height - player_2.height) / 2;
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

		if (multiplayer_action)// activates when multiplayer pressed, resets scores, object locations, toggles between multiplayer and cpu opponent
		{
			player_score = 0;
			cpu_score = 0;
			ball.Reset();
			if (multiplayer_off) {
				multiplayer_off = false;
				PlaySound(fx_Pog);
				player.y = (screen_height -player.height) / 2;
				player_2.y = (screen_height - player_2.height) / 2;
			}
			else {
				PlaySound(fx_Button);
				multiplayer_off = true;
				player.y = (screen_height - player.height) / 2;
				cpu.y = (screen_height - cpu.height) / 2;
			}
		}

		//menu toggle with space bar
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

		//begins drawing
		BeginDrawing();	
		
		if (isInMenu) //if in menu
		{
			//drawing menu assets 
			ClearBackground(Dark_Purple);
			DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Purple);
			DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Purple);
			DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
			DrawText("Ponggers Brother!!", (screen_width- MeasureText("Ponggers Brother!!", 80))/2 ,20, 80, WHITE);
			DrawText("space to toggle menu", 20,screen_height - 40, 20, WHITE);
			DrawText("Player 1 keys: up & down", screen_width - 300, screen_height - 80, 20, WHITE);
			DrawText("Player 2 keys: w & s", screen_width - 300, screen_height - 40, 20, WHITE);
			
			//Buttons (potential improvement: add a button class with subclasses for each button (decided to finish project and keep this in mind for next time)) 
			//play button
			if (play_state)//play state button and text with changing colour when hovered
			{
				DrawRectangle((screen_width - button_width) / 2, (screen_height - button_height) / 3, button_width, button_height, BLACK);
			}
			else
			{
				DrawRectangle((screen_width - button_width) / 2, (screen_height - button_height) / 3, button_width, button_height, WHITE);
			}
			DrawText("Play!", (screen_width - MeasureText("Play!", 80)) / 2, (screen_height - button_height) / 3 + 10, 80, Purple);

			//poggers button
			if (pog_state)
			{
				DrawRectangle((screen_width - button_width) / 2, (screen_height - button_height) / 2, button_width, button_height, BLACK);
			}
			else 
			{
				DrawRectangle((screen_width - button_width) / 2, (screen_height - button_height) / 2, button_width, button_height, WHITE);
			}
			DrawText("Poggers!", (screen_width - MeasureText("Poggers!", 80)) / 2, (screen_height - button_height) / 2 + 10, 80, Purple);

			//reset button
			if (reset_state)
			{
				DrawRectangle((screen_width - button_width) / 2, 2 * (screen_height - button_height) / 3, button_width, button_height, BLACK);
			}
			else
			{
				DrawRectangle((screen_width - button_width) / 2, 2 * (screen_height - button_height) / 3, button_width, button_height, WHITE);
			}
			DrawText("Reset", (screen_width - MeasureText("Reset", 80)) / 2, 2 * (screen_height - button_height) / 3 + 10, 80, Purple);
			
			//multiplayer/cpu button
			if (multiplayer_off) {
				if (multiplayer_state)
				{
					DrawRectangle((screen_width - button_width) / 2, 5 * (screen_height - button_height) / 6, button_width, button_height, BLACK);
				}
				else
				{
					DrawRectangle((screen_width - button_width) / 2, 5 * (screen_height - button_height) / 6, button_width, button_height, WHITE);
				}
				DrawText("2 Player", (screen_width - MeasureText("2 Player", 80)) / 2, 5 * (screen_height - button_height) / 6 + 10, 80, Purple);
			}
			else {
				if (multiplayer_state)
				{
					DrawRectangle((screen_width - button_width) / 2, 5 * (screen_height - button_height) / 6, button_width, button_height, BLACK);
				}
				else
				{
					DrawRectangle((screen_width - button_width) / 2, 5 * (screen_height - button_height) / 6, button_width, button_height, WHITE);
				}
				DrawText("CPU", (screen_width - MeasureText("CPU", 80)) / 2, 5 * (screen_height - button_height) / 6 + 10, 80, Purple);
			}
			
			//displays poggers image if poggers state is on 
			if (pog_on) {
				DrawTexture(texture_menu, 5 * screen_width / 6 - 150, screen_height / 2 - 150, WHITE);
				if (!multiplayer_off) {
					DrawTexture(texture_menu, screen_width / 6 - 150, screen_height / 2 - 150, WHITE);
				}
			}
		}
		else// playing the game, not in menu
		{
			//Updating positions
			ball.Update(fx_Score);
			player.Update();
			if (multiplayer_off)// cpu playing or player 2 
			{
				cpu.Update(ball.y);
			}
			else
			{
				player_2.Update();
			}

			//Check for collision ball and player
			if (CheckCollisionCircleRec(Vector2{ ball.x,ball.y }, ball.radius, Rectangle{ player.x,player.y,float(player.width),float(player.height) }))
			{
				ball.speed_x *= ball.speed_x_bounce; // reflection on hit, also increases x speed 
				ball.speed_y *= ball.speed_y_bounce[GetRandomValue(0, 1)];
				ball.rotation_speed *= ball.rotation_multiply[GetRandomValue(0, 5)];
				PlaySound(fx_Pong);
			}

			if (multiplayer_off) 
			{
				//Check for collision ball and cpu
				if (CheckCollisionCircleRec(Vector2{ ball.x,ball.y }, ball.radius, Rectangle{ cpu.x,cpu.y,float(cpu.width),float(cpu.height) }))
				{
					ball.speed_x *= ball.speed_x_bounce;
					ball.speed_y *= ball.speed_y_bounce[GetRandomValue(0, 1)];
					ball.rotation_speed *= ball.rotation_multiply[GetRandomValue(0, 5)];
					PlaySound(fx_Pong);

				}
			}
			else
			{
				//Check for collision ball and player_2
				if (CheckCollisionCircleRec(Vector2{ ball.x,ball.y }, ball.radius, Rectangle{ player_2.x,player_2.y,float(player_2.width),float(player_2.height) }))
				{
					ball.speed_x *= ball.speed_x_bounce;
					ball.speed_y *= ball.speed_y_bounce[GetRandomValue(0, 1)];
					ball.rotation_speed *= ball.rotation_multiply[GetRandomValue(0, 5)];
					PlaySound(fx_Pong);

				}
			}

			//Drawing background
			ClearBackground(Dark_Purple);
			DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Purple);
			DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Purple);
			DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
			
			//Drawing UI
			DrawText(TextFormat("%i", cpu_score), screen_width / 4 - 20, 20, 80, WHITE);
			DrawText(TextFormat("%i", player_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);

			//Drawing objects
			ball.Draw(texture, pog_on);
			player.Draw();
			if (multiplayer_off) // cpu or player 2 
			{
				cpu.Draw();
			}
			else
			{
				player_2.Draw();
			}
		}
		//DrawFPS(10, 10); //displays FPS to test
		EndDrawing();
	}
	CloseWindow();
}