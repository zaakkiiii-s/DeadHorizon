#include "raylib.h"
#include "raymath.h"
#include "sprite.hpp"
#include "DialogueBox.hpp"
#include <iostream>
#include <vector>
#include "Game.h"
#include "Ch2Game.h"
#include "Ch3Game.h"
#include <sstream>

using namespace std;


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
typedef enum GameScreen { START = 0, LOAD_CH1, CH1_RESTART, CH1_INTRO, CHAPTER1, CH2_RESTART, CH2_INTRO, CHAPTER2, CH3_RESTART, CH3_INTRO, CHAPTER3, PAUSE, ENDING } GameScreen;		// Creates a list of the different gamescreens

void DrawLoading(int& phase, int& loadTime, vector<const char*> lines, int& line);
void DrawCH1Intro();
void DrawCH2Intro();
void DrawCH3Intro();

void DrawEnding(const char* names[], int numNames, int& credit);
char* Wrap(const char* text, int Width, int fontsize);
bool DrawTextButton(Rectangle bounds, const char* text, int fontSize, Color normalColor, Color hoverColor, Color textColor);

int endingY = 800;

int main(void)
{
	// Initialization
	//--------------------------------------------------------------------------------------

	ZombieGame::Game game;
	Chapter2::Ch2Game ch2Game;
	Chapter3::Game ch3Game;

	game.setDboxImage();
	ch2Game.SetDboxImage();
	ch3Game.setDboxImage();

	const int screenWidth = 1200;
	const int screenHeight = 800;
	InitAudioDevice();
	Music chapter1Music = LoadMusicStream("Music_Resources/Chapter1Music.mp3");
	Music chapter2Music = LoadMusicStream("Music_Resources/Chapter2music.mp3");
	Music chapter3Music = LoadMusicStream("Music_Resources/Chapter2music.mp3");
	Music currentMusic = { 0 };

	
	SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

	GameScreen currentScreen = START;

	button startButton{ "Image_Resources/PlayButton.png", {(screenWidth / 2) - (float(135) / float(2)), 400}, 1 };		//Create a start button
	button quitButton{ "Image_Resources/CloseButton.png", {(screenWidth / 2) - (float(135) / float(2)), 600}, 1 };      //Create a quit button         

	Image image = LoadImage("Image_Resources/post_zombie_apocalyptic_city_t_resized.jpeg");		// Loads background image into memory
	Image ch1Image = LoadImage("Image_Resources/Chapter1_Background.png");		// Loads background image for chapter 1
	Texture2D Background_Titlescreen = LoadTextureFromImage(image);		// Loads background image as texture
	Texture2D Chapter1_Background = LoadTextureFromImage(ch1Image);

	
	const char* names[] = {"Zaakirah Shaik"};
	const int numNames = sizeof(names) / sizeof(names[0]);
	int phase = 0;
	int LoadTime = 0;
	int line = 0;
	int credit = 0;
	int EnterClicked = 0;
	bool startGame = false;
	vector<const char*> storyLine = { "The world has been overrun by zombies", "It is our job to keep what's left of humanity safe", "Now wake up and give them Hell!!!" };
	bool startButtonHovered = false;
	bool closeButtonHovered = false;
	bool isSoundOn = true;
	bool isPaused = false;
	GameScreen prevScreen = START;

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Only allow 'M' key to toggle pause if in active gameplay screen
		if (IsKeyPressed(KEY_M) && (currentScreen == CHAPTER1 || currentScreen == CHAPTER2 || currentScreen == CHAPTER3 || currentScreen == PAUSE)) {
			isPaused = !isPaused;
			if (isPaused) {
				prevScreen = currentScreen;
				currentScreen = PAUSE;
			}
			else {
				currentScreen = prevScreen;
			}
		}


		
		Music newMusic = currentMusic;

		if (currentScreen == CHAPTER1) newMusic = chapter1Music;
		else if (currentScreen == CHAPTER2) newMusic = chapter2Music;
		else if (currentScreen == CHAPTER3) newMusic = chapter3Music;
		else newMusic = { 0 }; 

		if (newMusic.stream.buffer != currentMusic.stream.buffer) {
			if (IsMusicStreamPlaying(currentMusic)) StopMusicStream(currentMusic);
			if (newMusic.stream.buffer != 0) PlayMusicStream(newMusic);
			currentMusic = newMusic;
		}

		if (IsMusicStreamPlaying(currentMusic)) UpdateMusicStream(currentMusic);
		SetMusicVolume(currentMusic, isSoundOn ? 1.0f : 0.0f);


		switch (currentScreen)			// Switch statement for chapter-specific variables, and function calls
		{

		case START:		//Case for the start screen of game. Inludes all variables and objects used in the start screen
		{
			Vector2 mousePosition = GetMousePosition();                     // Get vector position of mouse click
			bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);    // Get boolean value for button being pressed

			if (startButton.isPressed(mousePosition, mousePressed))         // Displays text to console if the button is pressed
			{
				currentScreen = LOAD_CH1;	// When start button pressed, switch screens to the screen for chapter 1 
			}

			if (quitButton.isPressed(mousePosition, mousePressed)) {
				CloseWindow();	// Close window when quit button is pressed
			}

			Vector2 mousePos = GetMousePosition();
			Rectangle startRec = { (screenWidth / 2) - (float(135) / float(2)), 400, startButton.getWidth(), startButton.getHeight() };
			startButtonHovered = CheckCollisionPointRec(mousePos, startRec);

			Rectangle closeRec = { (screenWidth / 2) - (float(135) / float(2)), 600, quitButton.getWidth(), quitButton.getHeight() };
			closeButtonHovered = CheckCollisionPointRec(mousePos, closeRec);

		}break;
		case LOAD_CH1:
		{
			if ((IsKeyPressed(KEY_SPACE)) && EnterClicked >= 3)
			{
				currentScreen = CH1_RESTART;
			}
			if (IsKeyPressed(KEY_ENTER))
			{
				line++;
				if (line == 3)
				{
					line = 0;
					startGame = true;
				}
				EnterClicked++;
			}

		}break;
		case CH1_RESTART:
		{

			game.drawChapter1Restart();


			Rectangle continueBtn = { 470, 600, 260,50  };
			bool hovered = CheckCollisionPointRec(GetMousePosition(), continueBtn);

			if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				currentScreen = CH1_INTRO;
			}


		} break;
		case CH1_INTRO:
		{
			if (IsKeyPressed(KEY_SPACE))
			{
				currentScreen = CHAPTER1;

			}

		}break;
		case CHAPTER1:
		{
			//Initialize any variable for the chapter 1 segment here
			if (game.chapterOver())
			{
				currentScreen = CH2_RESTART;
			}

			if (IsKeyPressed(KEY_W))		// Manual trigger for debugging, remove in final code
			{
				currentScreen = CH2_RESTART;
			}

			if (IsKeyPressed(KEY_P))
			{
				currentScreen = CH3_RESTART;
			}

			if (IsKeyPressed(KEY_E))
			{
				currentScreen = ENDING;
			}

		}break;
		case CH2_RESTART:
		{

			ch2Game.DrawChapterRestartScreen();


			Rectangle continueBtn = { 470, 600, 260,50 };
			bool hovered = CheckCollisionPointRec(GetMousePosition(), continueBtn);

			if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				currentScreen = CH2_INTRO;
			}
			if (IsKeyPressed(KEY_P))
			{
				currentScreen = CHAPTER3;
			}
		} break;
		case CH2_INTRO:
		{
			if (IsKeyPressed(KEY_SPACE))
			{
				currentScreen = CHAPTER2;
			}

			if (IsKeyPressed(KEY_P))
			{
				currentScreen = CH3_RESTART;
			}
		}break;
		case CHAPTER2:
		{
			if (ch2Game.isChapterOver())
			{
				//ch3Game = Chapter3::Game();
				currentScreen = CH3_RESTART;
			}

			if (IsKeyPressed(KEY_P))
			{
				currentScreen = CH3_RESTART;
			}

		}break;
		case CH3_RESTART:
		{
			// Area to call chapter 3 restart screen
			ch3Game.DrawChapterStartScreen();

			Rectangle restartBtn = {  470, 600, 260,50  };
			bool hovered = CheckCollisionPointRec(GetMousePosition(), restartBtn);

			if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				currentScreen = CH3_INTRO;
			}

		}break;
		case CH3_INTRO:
		{

			if (IsKeyPressed(KEY_SPACE))
			{
				currentScreen = CHAPTER3;
			}

		}break;
		case CHAPTER3:
		{
			if (ch3Game.isChapterOver() && IsKeyPressed(KEY_ENTER)) {
				currentScreen = ENDING;

			
			}

		}break;
		case ENDING:
		{

		}break;

		}

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(RAYWHITE);
		switch (currentScreen)
		{

		case START:
		{
			DrawTexture(Background_Titlescreen, 0, 0, WHITE);						// Draws background for titlescreen
			//DrawText("DEAD HORIZON", (screenWidth / 2 - 140), 100, 50, BLACK);
		
			//DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, BLACK);	// Draws 2 lines to accurately see center of the screen
			//DrawLine(0, screenHeight / 2, screenWidth, screenHeight / 2, BLACK);

			const char* title = "DEAD HORIZON";
			int fontSize = 70;
			int textWidth = MeasureText(title, fontSize); 
			int posX = (screenWidth / 2) - (textWidth / 2); 
			int posY = 100; 
			DrawText(title, posX + 5, posY + 5, fontSize, RED);
			DrawText(title, posX + 4, posY + 4, fontSize, RED);
			DrawText(title, posX + 3, posY + 3, fontSize, RED);
			DrawText(title, posX + 2, posY + 2, fontSize, BLACK);
			DrawText(title, posX + 1, posY + 1, fontSize, BLACK); 
			DrawText(title, posX, posY, fontSize, BLACK);

			startButton.Draw();		// Draws start button 
			quitButton.Draw();		// Draws quit button


			if (startButtonHovered)
			{
				DrawRectangleLines(530, 395, startButton.getWidth() + 10, startButton.getHeight() + 10, WHITE);
			}

			if (closeButtonHovered)
			{
				DrawRectangleLines(530, 595, quitButton.getWidth() + 10, quitButton.getHeight() + 10, WHITE);
			}
		}break;
		case LOAD_CH1:
		{
			ClearBackground(BLACK);
			DrawLoading(phase, LoadTime, storyLine, line);

			if (startGame)
			{
				DrawText("Press spacebar to begin", GetScreenWidth() - 300, GetScreenHeight() - 30, 20, WHITE);
			}

		}break;
		case CH1_INTRO:
		{
			ClearBackground(BLACK);
			DrawCH1Intro();
			DrawText("Press spacebar to begin", GetScreenWidth() - 300, GetScreenHeight() - 30, 20, WHITE);

		}break;
		case CHAPTER1:
		{

			game.Run();
			if (game.DrawPauseButton()) {
				prevScreen = currentScreen;
				currentScreen = PAUSE;
				isPaused = true;
			}

		}break;
		case CH2_INTRO:
		{
			ClearBackground(BLACK);
			DrawCH2Intro();
			DrawText("Press spacebar to begin", GetScreenWidth() - 300, GetScreenHeight() - 30, 20, WHITE);

		}break;
		case CHAPTER2:
		{
			ch2Game.Run();
			if (game.DrawPauseButton()) {
				prevScreen = currentScreen;
				currentScreen = PAUSE;
				isPaused = true;
			}

		}break;
		case CH3_INTRO:
		{
			ClearBackground(BLACK);
			DrawCH3Intro();
			DrawText("Press spacebar to begin", GetScreenWidth() - 300, GetScreenHeight() - 30, 20, WHITE);

		}break;
		case CHAPTER3:
		{
			ch3Game.Run();
			if (game.DrawPauseButton()) {
				prevScreen = currentScreen;
				currentScreen = PAUSE;
				isPaused = true;
			}
			if (ch3Game.isChapterOver() && IsKeyPressed(KEY_ENTER)) {
				currentScreen = ENDING;


			}

		}break;
		case PAUSE:
		{
			ClearBackground(DARKGRAY);
			DrawText("GAME PAUSED", screenWidth / 2 - 140, 150, 40, WHITE);

			Rectangle resumeBtn = { screenWidth / 2.0f - 100, 250, 200, 50 };
			Rectangle soundBtn = { screenWidth / 2.0f - 100, 320, 200, 50 };
			Rectangle mainMenuBtn = { screenWidth / 2.0f - 100, 390, 200, 50 };
			Rectangle quitBtn = { screenWidth / 2.0f - 100, 460, 200, 50 };

			if (DrawTextButton(resumeBtn, "Resume", 20, GRAY, LIGHTGRAY, BLACK)) {
				currentScreen = prevScreen;
				isPaused = false;
			}
			
			
			const char* soundLabel = isSoundOn ? "Sound: On" : "Sound: Off";

			if (DrawTextButton(soundBtn, soundLabel, 20, GRAY, LIGHTGRAY, BLACK)) {
				isSoundOn = !isSoundOn;
				
				if (isSoundOn) {
					cout << "ON"; 
				}
				else {
					cout << "OFF"; 
				}
				

			}


			if (DrawTextButton(mainMenuBtn, "Main Menu", 20, GRAY, LIGHTGRAY, BLACK)) {
				currentScreen = START;
				isPaused = false;
			}

			if (DrawTextButton(quitBtn, "Quit Game", 20, GRAY, LIGHTGRAY, BLACK)) {
				CloseWindow();
			}
		} break;
		case ENDING:
		{
			ClearBackground(BLACK);

			DrawEnding(names, numNames, credit);

		}break;

		}

		EndDrawing();
		//----------------------------------------------------------------------------------
	}
	UnloadMusicStream(chapter1Music);
	UnloadMusicStream(chapter2Music);
	UnloadMusicStream(chapter3Music);
	UnloadTexture(Background_Titlescreen);
	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseAudioDevice();
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}


void DrawLoading(int& phase, int& loadTime, vector<const char*> lines, int& line)
{
	switch (phase) {
	case 0:
	{
		DrawText("LOADING", 5, GetScreenHeight() - 30, 20, WHITE);
		if (loadTime % 60 == 0)
		{
			phase++;
		}
	}break;
	case 1:
	{
		DrawText("LOADING .", 5, GetScreenHeight() - 30, 20, WHITE);
		if (loadTime % 60 == 0)
		{
			phase++;
		}
	}break;
	case 2:
	{
		DrawText("LOADING . .", 5, GetScreenHeight() - 30, 20, WHITE);
		if (loadTime % 60 == 0)
		{
			phase++;
		}
	}break;
	case 3:
	{
		DrawText("LOADING . . .", 5, GetScreenHeight() - 30, 20, WHITE);
		if (loadTime % 60 == 0)
		{
			phase = 0;
		}
	}break;
	}
	loadTime++;

	int centLine = (GetScreenWidth() - MeasureText(lines[line], 30)) / 2;
	int centEnter = (GetScreenWidth() - MeasureText("Press Enter To Continue", 15)) / 2;

	DrawText(lines[line], centLine, 400, 30, WHITE);
	DrawText("Press Enter To Continue", centEnter, 700, 15, WHITE);
	DrawLine(500, 720, 710, 720, WHITE);

}
void DrawCH1Intro()
{
	DrawText("CHAPTER 1", 420, 40, 50, WHITE);
	DrawLine(400, 85, 720, 85, WHITE);

	Vector2 position = { 20, 300 };
	Vector2 origin = { 0, 0 };

	DrawTextPro(GetFontDefault(), "The bunker doors slam shut behind you. A grizzled survivalist named Cole \nwatches as you regain consciousness. \"Welcome to the last safe place \non Earth\", he says grimly. \"Out there, only the strong survive, but we need\npeople like you to recover and gather the strength to save one of our \nmen, Jacob. For you to do that, you will need to undergo training that \nconsists of questions about what you will encounter in the outside world \nright now and how to deal with it. If we think that you are ready, you will\nbe sent to save Jacob. Let us see if you are one of them.\"", position, origin, 0, 30, 3, WHITE);

}

void DrawCH2Intro()
{
	DrawText("CHAPTER 2", 420, 40, 50, WHITE);
	DrawLine(400, 85, 720, 85, WHITE);


	const char* intro = Wrap("You and a small group venture outside for the first time.The city is eerily silent. Buildings stand like tombstones, their windows shattered, their walls smeared with age - old bloodstains.The smell of decay lingers in the air. Sarah pulls her hoodie tighter around her face. \"We need supplies for Jacob. He`s trapped in a nearby building\", she whispers. Jacob was injured in a previous raid, and without medicine, he won`t make it. You grip your makeshift weapon tighter.This is your first real test.", 1100, 30);

	DrawText(intro, 10, 300, 30, WHITE);
}

void DrawCH3Intro()
{
	DrawText("CHAPTER 3", 420, 40, 50, WHITE);
	DrawLine(400, 85, 720, 85, WHITE);


	const char* intro = Wrap("The corridor reeks of decay and rusted metal. You push forward, heart pounding, until you see Jacob, slumped and barely conscious behind a barricaded door. Sarah catches her breath beside you. \"This is it,\" she whispers. But nothing is ever that simple. The way ahead is rigged with traps, and time is bleeding out fast. The decisions you make now will determine if Jacob makes it out alive or becomes another name etched into the ruins of this world. This is real. Let`s see if you are ready.", 1100, 30);

	DrawText(intro, 10, 300, 30, WHITE);
}

void DrawEnding(const char* names[], int numNames, int& credit)
{
	int centHeading = (1200 - MeasureText("Dead Horizon", 50)) / 2;

	DrawText("Dead Horizon", centHeading, endingY - 600, 50, WHITE);
	DrawLine(centHeading, endingY - 550, centHeading + MeasureText("Dead Horizon", 50), endingY - 550, WHITE);

	for (int i = 0; i < numNames; i++) {
		float yPos = endingY + 40 + i * 100;
		int cent = (1200 - MeasureText(names[i], 30)) / 2;
		DrawText(names[i], cent, yPos, 30, WHITE);
	}
	for (int i = 0; i < numNames; i++) {
		float yPos = endingY + 40 + i * 100;
		int cent = (1200 - MeasureText(names[i], 30)) / 2;
		DrawText(names[i], cent, yPos, 30, WHITE);
	}

	int centThanks = (1200 - MeasureText("Thank you for playing", 50)) / 2;
	DrawText("Thank you for playing", centThanks, endingY + 1000, 50, WHITE);

	endingY--;

}


char* Wrap(const char* text, int Width, int fontsize)
{
	string word, line, wrapped;
	int space = MeasureText(" ", fontsize);
	int lineWidth = 0;
	int wordWidth = 0;
	int length = strlen(text);
	char c;

	for (int i = 0; i < length; i++)
	{
		c = text[i];

		if ((c == ' ') || (c == '\n'))
		{
			wordWidth = MeasureText(word.data(), fontsize);

			if (lineWidth + wordWidth > Width)
			{
				wrapped += line + '\n';
				line = word;
				lineWidth = wordWidth + space;
			}
			else {
				if (!line.empty()) line += " ";
				line += word;
				lineWidth += wordWidth + space;
			}
			word = "";

			if (c == '\n') {
				wrapped += line + '\n';
				line.clear();
				lineWidth = 0;
			}

		}
		else {
			word += c;
		}

	}

	if (!word.empty()) {
		int wordWidth = MeasureText(word.data(), fontsize);
		if (lineWidth + wordWidth > Width) {
			wrapped += line + '\n';
			line = word;
		}
		else {
			if (!line.empty()) line += " ";
			line += word;
		}
	}

	if (!line.empty()) {
		wrapped += line;
	}

	length = wrapped.length();
	char* wrappedtext = new char[length + 1];
	copy(wrapped.begin(), wrapped.end(), wrappedtext);
	wrappedtext[length] = '\0';
	return wrappedtext;
}

bool DrawTextButton(Rectangle bounds, const char* text, int fontSize, Color normalColor, Color hoverColor, Color textColor)
{
	Vector2 mouse = GetMousePosition();
	bool isHovering = CheckCollisionPointRec(mouse, bounds);
	bool isClicked = isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

	// Draw button rectangle
	DrawRectangleRec(bounds, isHovering ? hoverColor : normalColor);

	// Center text
	int textWidth = MeasureText(text, fontSize);
	Vector2 textPos = {
		bounds.x + (bounds.width - textWidth) / 2,
		bounds.y + (bounds.height - fontSize) / 2
	};

	DrawText(text, textPos.x, textPos.y, fontSize, textColor);

	return isClicked;
}