#include "Ch3Game.h"
#include "Ch3Question.h"
#include <iostream>
#include "raylib.h"
#include "timer.h"
#include "ProgressBar.h"


using namespace std;


namespace Chapter3 {
	//Constructor intialises the gameclass for ch3. sets up the initial game state incle the question timer, player progress bar and UI layout position
	Game::Game() : currentQuestionIndex(0), score(0), selectedOption(-1), justEnteredPathway(false), enteredPathway(false), inPathway(false), questionTimer(600, 700, 40, 5, 1), timerExpired(false), 
		mainProgressBar(0, 770, 1205, 30, LIGHTGRAY, DARKPURPLE, Questions::mainQuestions, "PROGRESS..."),
		pathwayProgressBar(0, 770, 1200, 30, LIGHTGRAY, RED, Questions::pathwayQuestions, "PROGRESS...") {
		
		//Set Frame Rate for Chapter
		SetTargetFPS(60);	
		
		//Layout values for UI alignment
		horizontalLineY = GetScreenHeight() / 3.0f;
		verticalLineX = GetScreenWidth() * 0.75f;

		DialogueBox Dbox("", "", 10);
		Ch3Dbox = Dbox;

		// Load background textures
		Image img = LoadImage("Image_Resources/Chapter3Background.png");
		Ch3Background1 = LoadTextureFromImage(img);
		UnloadImage(img);
		Image img2 = LoadImage("Image_Resources/Chapter3Background2.png");
		Ch3Background2 = LoadTextureFromImage(img2);
		UnloadImage(img2);
		miniBackground = LoadTexture("wiresGameBackground.png");
		
		// Load gameplay objects
		redSocket = LoadTexture("socketR.png");
		characterSheet = LoadTexture("Image_Resources/Walk.png");
		pathway = LoadTexture("Image_Resources/Draft2.png");
		blueSocket = LoadTexture("socketB.png");
		yellowSocket = LoadTexture("socketY.png");
		wiresBackground = LoadTexture("Background5.png");
		
		//Load player sprite sheets
		background = LoadTexture("Image_Resources/forest_night_bg.png");
		camo = LoadTexture("Image_Resources/camo_netting.png");
	}
	//this fn controls the main gameplay loop for ch3, manages what is displayed on the screen based on the current game state
	void Game::Run() {
		
		//Clear the background
		ClearBackground(RAYWHITE);
		//changes the background after the 4th question
		if (currentQuestionIndex < 5)
		{
			DrawTexture(Ch3Background1, 0, 0, WHITE);
		}
		else
		{
			DrawTexture(Ch3Background2, 0, 0, WHITE);
		}
		//Draw chapter intro screen only in the beginning
		//shows the intro screen only once at the start
		if (isStart) {
			DrawChapterStartScreen();
			isStart = false;
		}
		//Show answer prompt screen
		//if a prompt is showing
		//draws the progres bar and prompt
		if (showingPrompt) {
			DrawProgress();
			DrawPromptScreen();

			//string::npos -> not dound when you search something in a string
			//if the @ char is found, enter a pathway
			if (waitingNextQuestion.find('@') != string::npos) {
				showingPrompt = false;
				enteredPathway = true;
			}
			else {
				//click Continue button
				//continue button triggers next quesution()
				Rectangle continueBtn = { GetScreenWidth() - 400, 340, 200, 40 };
				bool hovered = CheckCollisionPointRec(GetMousePosition(), continueBtn);

				if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					showingPrompt = false;
					nextQuestion();		//Advance to next question
				}
			}
		}
		// lets the player restart the current pathway if they fail.
		else if (enteredPathway) {
			DrawOptionRestartScreen();
			Rectangle restartBtn = { 470, 600, 260,50 };
			bool hovered = CheckCollisionPointRec(GetMousePosition(), restartBtn);

			//if player clicks the restart button
			if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				//resets values
				currentQuestionIndex = pathwayOrigin;//jumps player back to the start of the pathway
				inPathway = false;
				enteredPathway = false;
				waitingNextQuestion.clear();
				selectedOption = -1;//resets the players selected option to nothing
				isGameOver = false;// resets gameOver state, so they can re try
			}
		}
		//If boolean is true DrawConnectWires
		//draws prompt
		else if (showingConnectWiresPrompt) {
			DrawConnectWiresPrompt();
		}
		//If boolean is true DrawCamoPrompt
		//draws prompt
		else if (showingFindCamoPrompt ) {
			DrawCamoPrompt();
		}
		//Display MiniGame
		else if (showingFindCamoGame) {
			//will return true is the player wins
			bool success = PlayCamoGame();
			//if the player wins
			if (success) {
				showingFindCamoGame = false;
				showingFindCamoPrompt = false;
				nextQuestion();		//Advance after game
				currentQuestionIndex = 5;
				Ch3Dbox.NextPage();		//Dialogue box update
			}
			//if player doesnt win. returns to the prompt so they can try again
			else {
				showingFindCamoPrompt = true ;
			}
		}
		//Display MiniGame
		else if (showingConnectWiresGame) {
			DrawTexture(wiresBackground, 0, 0, WHITE);
			bool success = PlayConnectWiresGame();
			if (success) {
				showingConnectWiresGame = false;
				showingConnectWiresPrompt = false;
				nextQuestion();		//Advance after game. proceeds to next question
				currentQuestionIndex = 6;
				Ch3Dbox.NextPage();		//Dialogue box update
			}
			//if player loses, returns to the prompt screen
			else {
				showingConnectWiresPrompt = true;
			}
		}

		//question flow
		//main question and answering phase for ch3.
		//if the player is still answering the questions and hasnt reached the end of the list
		else if (currentQuestionIndex < GetCurrentQuestions().size()) {
			//question will only be showed if the timer hasnt expired and the time up screen isnt being shown yet.
			if (!timerExpired && !showTimeUpScreen) {
				DrawQuestionScreen();//draws current question and aswer options
				DrawProgress();//draws the progress bar
				//if the player clicks the mouse
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					//checks where the mouse is clicked
					Vector2 mousePos = GetMousePosition();//gets the mouse position
					const auto& currentQuestion = GetCurrentQuestions()[currentQuestionIndex];// gets current questions options

					//loops through the answer options
					for (int i = 0; i < currentQuestion.options.size(); i++) {

						float yPos = horizontalLineY + 50 + i * 60;
						Rectangle optionRect = { 250, yPos + 100, 700, 30 };
						//checks if you clicked on one
						if (CheckCollisionPointRec(mousePos, optionRect)) {
							ProcessAnswer(i);		 //Process selected answer
							break;
						}
					}
				}
			}

			//Timer Functions
			//handles what happens when the timer runs out
			questionTimer.updateTimer();//updates countdown timer
			questionTimer.DrawTimer();// draws it on screen

			//if timer runs out then times up screen is shown
			if (questionTimer.getTimeLeft() <= 0 && !timerExpired) {
				timerExpired = true;
				showTimeUpScreen = true;
			}

			if (showTimeUpScreen) {
				DrawTimeUpScreen();
			}
		}
		//Boolean to show ending animations
		else if (showingEndingMove) {
			bool completed = EndingMovement();
			if (completed) {
				showingEndingMove = false;
				chapterOver = true;
			}
		}
		//Final ending prompt
		else if (showingEndingPrompt)  {
			DrawEndingPrompt(); 

		}

		if (showingEndingMove) {
			EndingMovement();
		}
	
	}

	//Images for DialogBox
	void Game::setDboxImage()
	{
		Ch3Dbox.setBoxImage("Image_Resources/GreenTextBox.png");
		Ch3Dbox.setImage("Image_Resources/SarahCh3.png");
	}

	//Drawing prompt screen for ConnectWiresGame 
	void Game::DrawConnectWiresPrompt()
	{
		// Draw scaled background to fit the full screen
		DrawTexturePro(
			miniBackground,
			Rectangle{ 0, 0, (float)miniBackground.width, (float)miniBackground.height },
			Rectangle{ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
			Vector2{ 0, 0 },
			0.0f,
			WHITE
		);
		//draws dark transaprent overlay
		DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{ 0, 0, 0, 150 });

		//Text narrative
		//display text
		const char* line1 = "You stand before the locked doors of VirexCorp.";
		const char* line2 = "To gain entry, you must reconnect the severed wires";
		const char* line3 = "and restore power to the system.";
		const char* line4 = "\nRemember red to red, blue to blue and yellow to yellow!";

		// Measure widths of each line for box centering
		int textWidth1 = MeasureText(line1, 30);
		int textWidth2 = MeasureText(line2, 30);
		int textWidth3 = MeasureText(line3, 30);
		int textWidth4 = MeasureText(line4, 30);

		int maxTextWidth = max(max(textWidth1, textWidth2), max(textWidth3,textWidth4));
		int boxWidth = maxTextWidth + 40;
		int boxHeight = 180;

		int boxX = GetScreenWidth() / 2 - boxWidth / 2;
		int boxY = 50;
		//create and draw text box
		DrawRectangle(boxX, boxY, boxWidth, boxHeight, Color{ 0, 0, 0, 200 });

		//Cenetering text inside the box 
		DrawText(line1, GetScreenWidth() / 2 - textWidth1 / 2, boxY + 30, 30, WHITE);
		DrawText(line2, GetScreenWidth() / 2 - textWidth2 / 2, boxY + 60, 30, WHITE);
		DrawText(line3, GetScreenWidth() / 2 - textWidth3 / 2, boxY + 90, 30, WHITE);
		DrawText(line4, GetScreenWidth() / 2 - textWidth4 / 2, boxY + 90, 30, WHITE);

		
		int btnx = (1200 - 300) / 2;
		int btny = (800 - 50) / 2;
		Rectangle startBtn = { btnx, btny, 300, 50 };//draw attempt task button
		bool hovered = CheckCollisionPointRec(GetMousePosition(), startBtn);
		//button changes colour when hovered
		DrawRectangleRec(startBtn, hovered ? ColorFromHSV(240,0.337,0.361) : BLACK);
		int cent = (300 - MeasureText("ATTEMPT TASK", 20)) / 2;
		DrawText("ATTEMPT TASK", cent + 450, startBtn.y + 15, 20, RED);

		// If clicked transition to the wires minigame
		if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			showingConnectWiresPrompt = false; //hides prompt
			showingConnectWiresGame = true;
		}
	}
	//returns true if won
	bool Game::PlayConnectWiresGame()
	{
		//Game state variables
		float timer = 10.0f;
		bool gameWon = false; //tracks if game was completed correctly
		bool gameOver = false; //tracks whether player ran out of time or finished

		//socket collisions
		//target sockets
		Rectangle RedWire = { 880, 100, 80, 40 };
		Rectangle BlueWire = { 550, 100, 80, 40 };
		Rectangle YellowWire = { 210, 100, 80, 40 };

		//defining draggable wire peices
		//creates a list vector of 3 draggable wire objects.
		vector<Wire> wires = {
			{{190, 600, 100, 50}, {}, false, false, RED},// starting at 190 and y = 600. not been dragged, hasnt been placed at correctc socket. colour
			{{900, 600, 100, 50}, {}, false, false, DARKBLUE},
			{{550, 600, 100, 50}, {}, false, false, YELLOW}
		};
		//defines tryagain button
		Rectangle tryAgain = { 500,700,200,50 }; //x y pos, width, height


		while (!gameWon) {
			//subtracyts time based on how long each frame takes
			float dt = GetFrameTime();
			if (!gameWon && !gameOver)
				timer = timer - dt;
			//once timer hits 0, game is marked as over
			if (timer <= 0 && !gameWon)
				gameOver = true;

			//Handle wire dragging & placement
			for (auto& wire : wires) {
				if (wire.placed) continue;
				
				if (!gameOver) {
				// if mouse prseed and its over the wire, wire.dragging is true
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					if (CheckCollisionPointRec(GetMousePosition(), wire.rect)) {
						wire.dragging = true;
						//calcs offset so the wire sticks to the mouse properly
						wire.offset = {
							  GetMousePosition().x - wire.rect.x,
							  GetMousePosition().y - wire.rect.y
						};
					}
				}
				//Stop dragging and attempt to place
				if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
					wire.dragging = false;
					//check if wires placed correctly
					if (&wire == &wires[0] && CheckWirePlace(wire, RedWire))
						wire.placed = true;
					else if (&wire == &wires[1] && CheckWirePlace(wire, BlueWire))
						wire.placed = true;
					else if (&wire == &wires[2] && CheckWirePlace(wire, YellowWire))
						wire.placed = true;
				}
				//update position while dragging
				if (wire.dragging) {
					wire.rect.x = GetMousePosition().x - wire.offset.x;
					wire.rect.y = GetMousePosition().y - wire.offset.y;
				}
			}
		}
			//check if game is won
			if (!gameWon && wires[0].placed && wires[1].placed && wires[2].placed) {
				gameWon = true;
			}
			//start drawing 
			BeginDrawing();
			ClearBackground(RAYWHITE);
			DrawTexture(wiresBackground, 0, 0, WHITE);

			//Title and timer
			int textWidth = MeasureText("Connect the Wires!", 30);
			DrawText("Connect the Wires!", (GetScreenWidth() - textWidth) / 2, 30, 30, BLACK);

			textWidth = MeasureText(TextFormat("Time Left: %.1f", timer), 30);
			DrawText(TextFormat("Time Left: %.1f", timer), (GetScreenWidth() - textWidth) / 2, 65, 30, BLACK);

			//Draw sockets
			DrawTexture(redSocket, RedWire.x, RedWire.y, WHITE);
			DrawTexture(blueSocket, BlueWire.x, BlueWire.y, WHITE);
			DrawTexture(yellowSocket, YellowWire.x, YellowWire.y, WHITE);
			//draws each draggable wire
			for (auto& item : wires) {
				DrawRectangleRec(item.rect, item.color);
			}

			//Fail screen and retry button
			if (gameWon)
				DrawText("Wires Connected Successfully! Access Granted", 250, 700, 30, BLACK);

			//If gameover is true draw Try again button
			if (gameOver) {
				int btnx = (1200 - 300) / 2;
				int btny = (800 - 50) / 2;
				Rectangle startBtn = { btnx, btny, 300, 50 };
				bool hovered = CheckCollisionPointRec(GetMousePosition(), startBtn);

				DrawRectangleRec(startBtn, hovered ? ColorFromHSV(240, 0.337, 0.361) : BLACK);
				int cent = (300 - MeasureText("TRY AGAIN", 20)) / 2;
				DrawText("TRY AGAIN", cent + 450, startBtn.y + 15, 20, WHITE);

				//restart logic
				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), startBtn)) {
					//calls the reset function
					Reset(timer, gameWon, gameOver, wires);
				}
			}
			//Stop drawing
			EndDrawing();
		}
		return gameWon;
	}

	//collision check moveable rectangle and fixed socket
	bool Game::CheckWirePlace(Wire& wire, Rectangle target)
	{
		return CheckCollisionRecs(wire.rect, target);
	}

	//Timer reset and game state 
	void Game::Reset(float& timer, bool& gameWon, bool& gameOver, vector<Wire>& wires)
	{
		timer = 10.0f;
		gameWon = false;
		gameOver = false;
		//Reset position of wires
		wires[0].rect = { 210,600,100,50 };
		wires[1].rect = { 880, 600,100,50 };
		wires[2].rect = { 550,600,100,50 };

		//clear flags
		for (auto& wire : wires) {
			wire.placed = false;
		}
	}

	void Game::DrawCamoPrompt()
	{
		//Draw background
		DrawTexturePro(
			miniBackground,
			Rectangle{ 0, 0, (float)miniBackground.width, (float)miniBackground.height },
			Rectangle{ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
			Vector2{ 0, 0 },
			0.0f,
			WHITE
		);
		//draw a dark semi transparent overlay
		DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{ 0, 0, 0, 150 });

		//Text Narrative
		const char* line1 = "Just as the first light breaks through the trees,";
		const char* line2 = "Sarah spots something hidden among the brush.";
		const char* line3 = "Assist Sarah in finding the camo netting.";
		const char* line4 = "You need to find the camo in the allocated time!!";

		//Measure for centering text
		//calculates the width of each line in pixels whic is needed for centering 
		int textWidth1 = MeasureText(line1, 30);
		int textWidth2 = MeasureText(line2, 30);
		int textWidth3 = MeasureText(line3, 30);
		int textWidth4 = MeasureText(line4, 30);

		//finds the widest line of text and sets the box width slightly wider
		int maxTextWidth = max(max(textWidth1, textWidth2), max(textWidth3,textWidth4));
		int boxWidth = maxTextWidth + 40;
		int boxPadding = (1200 - boxWidth) / 2;//calcs how much space is left on each side
		int boxHeight = 210;


		//Draw dialogue box
		//centers the box
		int boxX = GetScreenWidth() / 2 - boxWidth / 2;
		int boxY = 50;
		//fills it with a transparent dark colour
		DrawRectangle(boxX, boxY, boxWidth, boxHeight, Color{ 0, 0, 0, 200 });

		// Center and draws each line of text
		DrawText(line1, ((boxWidth - textWidth1) / 2) + boxPadding, boxY + 30, 30, WHITE);
		DrawText(line2, ((boxWidth - textWidth2) / 2) + boxPadding, boxY + 70, 30, WHITE);
		DrawText(line3, ((boxWidth - textWidth3) / 2) + boxPadding, boxY + 110, 30, WHITE);
		DrawText(line4, ((boxWidth - textWidth4) / 2) + boxPadding, boxY + 150, 30, WHITE);

	
		int btnx = (1200 - 420) / 2;
		int btny = (800 - 50) / 2;

		//defines the start button
		Rectangle startBtn = { btnx, btny, 420, 50 };
		////checks if the mouse is over the button
		bool hovered = CheckCollisionPointRec(GetMousePosition(), startBtn);

		//draw the button with the hover effect
		DrawRectangleRec(startBtn, hovered ? ColorFromHSV(240, 0.337, 0.361) : BLACK);
		int cent = (420 - MeasureText("HELP SARAH FIND THE CAMO FENCE", 20)) / 2;
		DrawText("HELP SARAH FIND THE CAMO FENCE", cent + 390, startBtn.y + 15, 20, RED);

		//transition to Camo game
		if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			showingFindCamoPrompt = false;
			showingFindCamoGame = true; 
		}
	}

	bool Game::PlayCamoGame()
	{
		// Game loop for camo game
	while (!foundCamo) {
		
		//gets the time in seconds since the last frame. used for smooth countdown timer updates
		float delta = GetFrameTime(); // Time elapsed per frame

		//sets the position of the camo name	
		if (camoPos.x == 0 && camoPos.y == 0) {
			camoPos = {
				(float)(1200 - camoWidthScaled - 40),
				(float)(800 - camoHeightScaled - 40)
			};
		}
		//if the game is not over. decrease the timer.
		if (!gameOver) {
			gameTimer -= delta;

			// Check for mouse click
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				Vector2 mouse = GetMousePosition();
				//draw rectangle camo area
				Rectangle camoArea = { camoPos.x, camoPos.y, (float)camoWidthScaled, (float)camoHeightScaled };


				// If the player clicks inside the camo object trigger found 
				if (CheckCollisionPointRec(mouse, camoArea)) {
					foundCamo = true;
					gameOver = true;
					lastRestartTime = GetTime();
				}
			}
			// If the timer expires
			if (gameTimer <= 0) {
				gameOver = true;
				foundCamo = false;		//trigger failure
				lastRestartTime = GetTime();
			}
		}
		else {
			if (!foundCamo) {
				//defines the restart button
				Rectangle restartBtn = { 1200 / 2 - 80, 800 / 2 + 50, 160, 40 };
				
				//restart the game by clicking the restart button
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					Vector2 mouse = GetMousePosition();
					if (CheckCollisionPointRec(mouse, restartBtn)) {
						gameTimer = timerLimit;//resets the time limit 
						gameOver = false;
						foundCamo = false;
					}
				}
			}
			else {
				if (GetTime() - lastRestartTime > 3.0) {
					gameTimer = timerLimit;
					gameOver = false;
					foundCamo = false;
				}
			}
		}

		BeginDrawing();
		ClearBackground(BLACK);

		//draws the title text
		const char* gameTitle = "Find the Camo Netting!";
		int titleFontSize = 50;
		//meausres the width and height so it can be centered
		int titleWidth = MeasureText(gameTitle, titleFontSize);
		DrawText(gameTitle, (1200 - titleWidth) / 2, 20, titleFontSize, WHITE);

		// Draw game background
		DrawTexturePro(
			background,
			{ 0, 0, (float)background.width, (float)background.height },
			{ 0, 0, (float)1200, (float)800 },
			{ 0, 0 },
			0.0f,
			WHITE
		);

		// Draw the camo netting
		DrawTexturePro(
			camo,
			{ 0, 0, (float)camo.width, (float)camo.height },
			{ camoPos.x, camoPos.y, (float)camoWidthScaled, (float)camoHeightScaled },
			{ 0, 0 },
			0.0f,
			WHITE
		);

		// Draw timer box
		int boxx = (1200 - 220) / 2;
		DrawRectangle(boxx, 10, 220, 40, Fade(BLACK, 0.6f));
		int cent = (1200 - MeasureText("Time Left: 00", 25)) / 2;
		DrawText(TextFormat("Time Left: %02d", (int)gameTimer), cent, 20, 25, WHITE);

		//Display text if camo was found 
		if (gameOver) {
			if (foundCamo) {
				DrawText("You found it!", 100, 800 / 2, 20, GREEN);
			}
			//if player ran out of time 
			else {
				
				int cent = (1200 - MeasureText("You were too slow!",35)) / 2;
				DrawText("You were too slow!", cent, 300, 35, RED);
		
				Rectangle restartBtn = { (1200 - 160) / 2 , 800 / 2 + 50, 160, 50 };
				bool hovered = CheckCollisionPointRec(GetMousePosition(), restartBtn);

				//draw restart button
				DrawRectangleRec(restartBtn, hovered ?   BLACK : ColorFromHSV(240, 0.337, 0.361));
				cent = (160 - MeasureText("RESTART", 20)) / 2;
				DrawText("RESTART", cent + 520, restartBtn.y + 15, 20, WHITE);
			}
		}

		EndDrawing();
		}
		// exit condition if camo found
		return foundCamo ;  
	}
	
	//Draw the prompt before ending
	void Game::DrawEndingPrompt()
	{
		// Draw the background texture
		DrawTexturePro(
			pathway,
			Rectangle{ 0, 0, (float)miniBackground.width, (float)miniBackground.height },
			Rectangle{ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
			Vector2{ 0, 0 },
			0.0f,
			WHITE
		);
		//draws a translucent black overlay
		DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{ 0, 0, 0, 150 });

		//narrative text 
		const char* line1 = "A big decision has been made";
		const char* line2 = "Help deliver the cure";
		const char* line3 = "and lets see the world change";
		const char* line4 = "\nUse A and D to move the character";
		
		//Get width for centering 
		int textWidth1 = MeasureText(line1, 30);
		int textWidth2 = MeasureText(line2, 30);
		int textWidth3 = MeasureText(line3, 30);
		int textWidth4 = MeasureText(line4, 30);

		int maxTextWidth = max(max(textWidth1, textWidth2), max(textWidth3, textWidth4));
		int boxWidth = maxTextWidth + 40;
		int boxHeight = 180;


		int boxX = GetScreenWidth() / 2 - boxWidth / 2;
		int boxY = 50;
		DrawRectangle(boxX, boxY, boxWidth, boxHeight, Color{ 0, 0, 0, 200 });
		
		// Draw centered text lines inside the box
		DrawText(line1, GetScreenWidth() / 2 - textWidth1 / 2, boxY + 30, 30, WHITE);
		DrawText(line2, GetScreenWidth() / 2 - textWidth2 / 2, boxY + 60, 30, WHITE);
		DrawText(line3, GetScreenWidth() / 2 - textWidth3 / 2, boxY + 90, 30, WHITE);
		DrawText(line4, GetScreenWidth() / 2 - textWidth4 / 2, boxY + 90, 30, WHITE);

		//draw the Deliver Cure button
		int btnx = (1200 - 300) / 2;
		int btny = (800 - 50) / 2;
		Rectangle startBtn = { btnx, btny, 300, 50 };
		bool hovered = CheckCollisionPointRec(GetMousePosition(), startBtn);

		// Draw the button with effect
		DrawRectangleRec(startBtn, hovered ? ColorFromHSV(240, 0.337, 0.361) : BLACK);
		int cent = (300 - MeasureText("Deliver Cure", 20)) / 2;
		DrawText("Deliver Cure", cent + 450, startBtn.y + 15, 20, RED);

		//Transition to the movement gameplay section
		if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			showingEndingPrompt = false;
			showingEndingMove = true;
		}


	}

	//Template collision checker 
	template <typename T>
	bool CheckCollision(T playerPosX, T obstaclePosX, const char* obstacleName) {
		if (playerPosX + 50 >= obstaclePosX - 50 && playerPosX <= obstaclePosX + 50) {
			DrawText(
				TextFormat("You delivered the cure!", obstacleName),
				20, 50, 30, RED
			);
			return true;
		}
		return false;
	}


	bool Game::EndingMovement()
	{
		// frame count according to sprite sheet
		const int frameCount = 7;// charachter sprite sheet has 7 frames
		int frameWidth = characterSheet.width / frameCount;
		int frameHeight = characterSheet.height;

		const float frameSpeed = 7.0f;	//frames per second

		const float scale = 3.0f;
		Vector2 position = { 100.0f, 800.0f - (frameHeight * scale) - 125 };		//starting position

		//movement speed - how fast the character moves
		const float moveSpeed = 150.0f;
		//for smooth movement
		float deltaTime = GetFrameTime();
		
		//handle movement
		while (!Excute) {
			//trigger forplayer movement
			bool isMoving = false;
			if (canMove && !cureDelivered) {
				//Movement to the right
				if (IsKeyDown(KEY_D)) {
					position.x += moveSpeed * deltaTime;
					isMoving = true;
				}
				//Movement to the left
				if (IsKeyDown(KEY_A)) {
					position.x -= moveSpeed * deltaTime;
					isMoving = true;
				}
			}

			//Boundaries between screen
			if (position.x < 0) position.x = 0;
			if (position.x > 1200 - frameWidth * scale) position.x = 1200 - frameWidth * scale;

			//Update animation frame
			if (isMoving) {
				frameTimer += deltaTime;
				if (frameTimer >= 1.0f / frameSpeed) {
					frameTimer -= 1.0f / frameSpeed;
					currentFrame++;
					if (currentFrame >= frameCount) currentFrame = 0;
				}
			}
			else {
				currentFrame = 0;
			}

			//which frame to draw
			Rectangle frameRec = {
				static_cast<float>(currentFrame * frameWidth),
				0.0f,
				static_cast<float>(frameWidth),
				static_cast<float>(frameHeight)
			};

			//Collision Point 
			const float ObstaclePos = 900.0f;
			//chack if cure is delivered
			cureDelivered = CheckCollision(position.x, ObstaclePos, "Delivered");		//calling template function

			//if collision met trigger movement off
			if (cureDelivered) {
				canMove = false;
				timer = true;
			}


			if (cureDelivered && timer && timerStart == 0.0f) {
				timerStart = GetTime();
			}

			//11 seconds after delivery. show the final screen
			if (timer && !gameOverEnd && GetTime() - timerStart >= 11.0f) {
				gameOverEnd = true;
			}

			//Begin Drawing
			BeginDrawing();
			if (gameOverEnd) {
				chapterOver = true;
				ClearBackground(BLACK);		// Clear the screen
				DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);

				// Title Box
				const char* title = "YOU HAVE COMPLETED DEAD HORIZON";
				int titleFontSize = 50;
				float titleWidth = (float)MeasureText(title, titleFontSize);

				float boxHeight = 100.0f;
				float boxY = (GetScreenHeight() - boxHeight) / 2.0f;

				//Dimensions
				Rectangle titleBox = {
					(GetScreenWidth() - titleWidth - 40.0f) / 2.0f,
					boxY,
					titleWidth + 40.0f,
					100.0f
				};


				DrawRectangleRec(titleBox, Color{ 0, 0, 0, 200 });
				DrawRectangleLinesEx(titleBox, 3.0f, WHITE);
				DrawText(title, (int)titleBox.x + 20, (int)titleBox.y + 30, titleFontSize, RED);
				//Display Press ENTER message
				DrawText("Press ENTER to go to credits", GetScreenWidth() - MeasureText("Press ENTER to go to credits",20)-5, GetScreenHeight() - 30, 20, WHITE);

				//exit loop if enter is pressed
				if (IsKeyPressed(KEY_ENTER)) {
					Excute = true;
					return true;
				}
			}
			else {
				//Fit pathway in background
				DrawTexturePro(
					pathway,
					Rectangle{ 0, 0, (float)pathway.width, (float)pathway.height },
					Rectangle{ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
					Vector2{ 0, 0 },
					0.0f,
					WHITE
				);

				//Drawing chraracter sprite with current animation
				DrawTexturePro(
					characterSheet,
					frameRec,
					Rectangle{ position.x, position.y, frameRec.width * scale, frameRec.height * scale },
					Vector2{ 0, 0 },
					0.0f,
					WHITE
				);

				//cureDelivered display prompt
				if (cureDelivered) {
					DrawText(currentPrompt.c_str(), 60 , 800 / 2 - 375 , 30 , BLACK);
				}
			}
			EndDrawing();
		}

		if (cureDelivered) {
			return cureDelivered;
			Excute = true; 
		}

	}

	//Boolean type to indicate when to go to the next stage
	bool Game::isChapterOver()
	{
		if (chapterOver) {
			return chapterOver;
		}	
	}

	//Returns the current set of questions
	const vector<Question>& Game::GetCurrentQuestions() const {
		//this fn selects which question set to use
		return inPathway ? Questions::pathwayQuestions : Questions::mainQuestions;
	}
	//fn parses a string like 5-b or 6 to determine where to go next in the question flow
	pair<bool, int> Game::ParseQuestionIndex(const string& next, int currentMainIndex) {
		if (next.empty()) {
			return make_pair(false, -1);		//Indicates the end of questions
		}

		//If the string contains '@' trigger pathway entry
		if (next.find('@') != string::npos) {
			enteredPathway = true;
			isGameOver = true;
			return make_pair(false, -1);
		}
		//mapping strings to pathway indices
		//if they chose 1-b, go to pathway index 0 etc
		if (next == "1-B") return make_pair(true, 0);
		if (next == "1-C") return make_pair(true, 1);
		if (next == "2-B") return make_pair(true, 2);
		if (next == "3-B") return make_pair(true, 3);
		if (next == "3-C") return make_pair(true, 4);
		if (next == "4-B") return make_pair(true, 5);
		if (next == "4-C") return make_pair(true, 6);
		if (next == "5-B") return make_pair(true, 7);
		if (next == "5-C") return make_pair(true, 8);
		if (next == "6-B") return make_pair(true, 9);
		if (next == "6-C") return make_pair(true, 10);
		if (next == "7-B") return make_pair(true, 11);
		if (next == "7-C") return make_pair(true, 12);

		return make_pair(false, atoi(next.c_str()) - 1);
	}

	void Game::ProcessAnswer(int selectedOption) {
		//Save the selected option
		this->selectedOption = selectedOption;//saves the players chosen option
		const auto& currentQuestion = GetCurrentQuestions()[currentQuestionIndex];///get the current question from the list of questions

		//For prompt
		waitingNextQuestion = currentQuestion.nextQuestions[selectedOption];// based on the option the player chose get the string for the next question
		//set prompt to display after answering
		currentPrompt = currentQuestion.prompts[selectedOption];
		showingPrompt = true;
		
		//If there is no next question end the game
		if (waitingNextQuestion.empty()) {
			currentQuestionIndex = (int)GetCurrentQuestions().size(); // End game
			showingPrompt = false;
			showingEndingPrompt = true;
			return;
		}

	}

	void Game::nextQuestion() {
		//question index is empty end game
		if (waitingNextQuestion.empty()) {
			currentQuestionIndex = (int)GetCurrentQuestions().size(); // End game
			return;
		}

		//Determine if the next question is part of pathway
		pair<bool, int> result = ParseQuestionIndex(waitingNextQuestion, inPathway ? pathwayOrigin : currentQuestionIndex);

		// Pathway questions 
		if (result.first) { 
			pathwayProgressBar += 1;					//move the progress bar for the pathway
			pathwayOrigin = currentQuestionIndex;
			currentQuestionIndex = result.second;		//Jump to pathway question
			inPathway = true;
		}
		// Main question
		else { 
			mainProgressBar += 1;		//for progress bar 
			if (result.second == -1) {
				currentQuestionIndex = (int)GetCurrentQuestions().size(); // End game
			}
			else {
				currentQuestionIndex = result.second;
				//show Find Camo prompt before question 5
				if (currentQuestionIndex == 5 && !showingFindCamoPrompt && !showingFindCamoGame) {
					showingFindCamoPrompt = true;
					currentQuestionIndex--;			//Stay on previous question

				}
				//show Connect Wires prompt before question 6
				else if (currentQuestionIndex == 6 && !showingConnectWiresPrompt && !showingConnectWiresGame) {
					showingConnectWiresPrompt = true;
					currentQuestionIndex--;			//Stay on previous question
				}	
				inPathway = false;
			}
		}
		//within bounds start the timer
		//if the questions havent ended
		//gets the time limit for the new questions
		if (currentQuestionIndex < GetCurrentQuestions().size()) {
			int questionTimeLimit = GetCurrentQuestions()[currentQuestionIndex].timelimit;
			questionTimer = Timer(600, 700, 40, questionTimeLimit, 1);
		}
		//Reset input state
		waitingNextQuestion.clear();
		selectedOption = -1;
		 
	}
	//only called once at the beginning when isStart == true
	void Game::DrawChapterStartScreen()
	{
		DrawTexture(Ch3Background1, 0, 0, WHITE);

		//Title setup
		const char* title = "CHAPTER 3: THE FINAL CHAPTER";
		int titleFontSize = 60;
		float titleWidth = (float)MeasureText(title, titleFontSize);

		//Centre title box
		Rectangle titleBox = {
			(GetScreenWidth() - titleWidth - 40.0f) / 2.0f,
			120.0f,
			titleWidth + 40.0f,
			100.0f
		};

		//Draw title box with text
		DrawRectangleRec(titleBox, Color{ 0, 0, 0, 200 });
		DrawRectangleLinesEx(titleBox, 3.0f, WHITE);
		DrawText(title, (int)titleBox.x + 20, (int)titleBox.y + 30, titleFontSize, WHITE);

		const char* message = "";
		message = " You must answer the questions in the allocated time";

		int msgFontSize = 30;
		float msgWidth = (float)MeasureText(message, msgFontSize);


		Rectangle msgBox = {
			(GetScreenWidth() - msgWidth - 40.0f) / 2.0f,
			280.0f,
			msgWidth + 40.0f,
			60.0f
		};

		Color boxColor = RED;
		DrawText(message, (int)msgBox.x + 20, (int)msgBox.y + 15, msgFontSize, WHITE);

		Rectangle restartBtn = {  470, 600, 260,50 };
		//check if mouse is hovering
		bool hovered = CheckCollisionPointRec(GetMousePosition(), restartBtn);
		DrawRectangleRec(restartBtn, hovered ? DARKGRAY : boxColor);

		const char* btnText;
		btnText = "START CHAPTER";

		
		int btnTextWidth = MeasureText(btnText, 20);
		DrawText(btnText,
			(int)(restartBtn.x + (restartBtn.width - btnTextWidth) / 2.0f),
			(int)restartBtn.y + 15,
			20,
			WHITE);

		
		if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			//Reset question index
			currentQuestionIndex = pathwayOrigin;
			inPathway = false;
			showingChapterRestartScreen = false;
			selectedOption = -1;

			//Start the timer for the next question
			int questionTimeLimit = GetCurrentQuestions()[currentQuestionIndex].timelimit;
			questionTimer = Timer(600, 700, 40, questionTimeLimit, 1);
			timerExpired = false;
			//Reset both progress bars
			mainProgressBar -= mainProgressBar.GetCurrentIndex();
			pathwayProgressBar -= pathwayProgressBar.GetCurrentIndex();
		}
	}
	//displays a message when the player doesnt answer in time. has a restart button
	void Game::DrawTimeUpScreen()
	{
		DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{ 0, 0, 0, 150 });
		const char* title = "CHAPTER 3: THE FINAL CHAPTER";
		int titleFontSize = 60;
		float titleWidth = (float)MeasureText(title, titleFontSize);

		// Title setup
		Rectangle titleBox = {
			(GetScreenWidth() - titleWidth - 40.0f) / 2.0f,
			120.0f,
			titleWidth + 40.0f,
			100.0f
		};

		//Draw title box and text
		DrawRectangleRec(titleBox, Color{ 0, 0, 0, 200 });
		DrawRectangleLinesEx(titleBox, 3.0f, WHITE);
		DrawText(title, (int)titleBox.x + 20, (int)titleBox.y + 30, titleFontSize, WHITE);

		const const char* message = "TIME'S UP!!";
		int msgFontSize = 30;
		float msgWidth = (float)MeasureText(message, msgFontSize);

		//Message box setup
		Rectangle msgBox = {
			(GetScreenWidth() - msgWidth - 40.0f) / 2.0f,
			280.0f,
			msgWidth + 40.0f,
			60.0f
		};

		const char* message2 = "You didn't answer in time!";
		
		int msgFontSize2 = 30;
		float msgWidth2 = MeasureText(message, msgFontSize);
		int cent = (GetScreenWidth() - MeasureText(message2, 30)) / 2;
		DrawText(message2, cent, GetScreenHeight() / 2 - 40, msgFontSize2, WHITE);


		Color boxColor = RED;
		DrawText(message, (int)msgBox.x + 20, (int)msgBox.y + 15, msgFontSize, RED);
		//Draw retry button 
		Rectangle restartBtn = { 470, 600, 260,50 };

		bool hovered = CheckCollisionPointRec(GetMousePosition(), restartBtn);
		DrawRectangleRec(restartBtn, hovered ? DARKGRAY : boxColor);
		
		//Centre retry button 
		const char* btnText = "RETRY";
		int btnTextWidth = MeasureText(btnText, 20);
		DrawText(btnText,
			(int)(restartBtn.x + (restartBtn.width - btnTextWidth) / 2.0f),
			(int)restartBtn.y + 15,
			20,
			WHITE);


		//On click go to main question, reset timer and flags for boolean types 
		if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			int questionTimeLimit = GetCurrentQuestions()[currentQuestionIndex].timelimit;
			questionTimer = Timer(600, 700, 40, questionTimeLimit, 1);
			timerExpired = false;
			showTimeUpScreen = false;
			selectedOption = -1;

		}

	}
	//shows a transition or consequence after answering a question
	void Game::DrawPromptScreen() {

		//Displays the prompts in dialogue box
		Ch3Dbox.setText(currentPrompt.c_str());
		Ch3Dbox.DrawDialogueBox(27, 30);

		// If the next question marker is not '@' show the "Continue" button 
		if (waitingNextQuestion.find('@') == string::npos) {
			Rectangle continueBtn = { GetScreenWidth() - 400, 340, 200, 40 };
			bool hovered = CheckCollisionPointRec(GetMousePosition(), continueBtn);
			int contCent = continueBtn.x + (200 - MeasureText("Continue", 20)) / 2;
			// Draw the button with a hover effect
			DrawRectangleRec(continueBtn, hovered ? DARKGRAY : BLACK);

			DrawText("Continue", contCent, continueBtn.y + 10, 20, WHITE);
			//transition to the dialogue box 
			Ch3Dbox.NextPage();

		}
	}
	//displays a question and clickable answer options
	// uses mouse clicks to detect theplayers answer

	void Game::DrawQuestionScreen() {
		const auto& currentQuestions = GetCurrentQuestions();

		//checks if currentQuestionIndex is out of bounds
		if (currentQuestionIndex < 0 || currentQuestionIndex >= currentQuestions.size()) {
			return;
		}

		const auto& currentQuestion = currentQuestions[currentQuestionIndex];

		// Set the dialogue text
		Ch3Dbox.setText(currentQuestion.text.c_str());
		Ch3Dbox.DrawDialogueBox(27, 30);

		//Draw each answer option
		for (int i = 0; i < currentQuestion.options.size(); i++) {
			float yPos = horizontalLineY + 40 + i * 60;

			bool hovered = CheckCollisionPointRec(GetMousePosition(), Rectangle{ 235, yPos + 100, 730, 30 });
			DrawRectangleRec(Rectangle{ 235, yPos + 110, 730, 30 }, hovered ? DARKBLUE : ColorFromHSV(238, 0.92, 0.36));

			int centText = 235 + (730 - MeasureText(currentQuestion.options[i].c_str(), 20)) / 2;
			DrawText(currentQuestion.options[i].c_str(), centText, yPos + 115, 20, WHITE);
		}
		//transition to the dialogue box 
		Ch3Dbox.NextPage();
	}
	// responsible for displaying the "Game Over" screen when the player fails
	void Game::DrawRestartPrompt() {

		DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);

		// Title Box
		const char* title = "GAME OVER";
		int titleFontSize = 60;
		float titleWidth = (float)MeasureText(title, titleFontSize);

		//centered rectangle box around the title
		Rectangle titleBox = {
			(GetScreenWidth() - titleWidth - 40.0f) / 2.0f,
			120.0f,
			titleWidth + 40.0f,
			100.0f
		};

		DrawRectangleRec(titleBox, Color{ 0, 0, 0, 200 });
		DrawRectangleLinesEx(titleBox, 3.0f, WHITE);
		DrawText(title, (int)titleBox.x + 20, (int)titleBox.y + 30, titleFontSize, RED);

		//Display End of Game Message from currentPrompt
		const char* message = currentPrompt.c_str();
		int msgFontSize = 30;
		float msgWidth = (float)MeasureText(message, msgFontSize);

		Rectangle msgBox = {
			(GetScreenWidth() - msgWidth - 40.0f) / 2.0f,
			280.0f,
			msgWidth + 40.0f,
			60.0f
		};

		DrawText(message, (int)msgBox.x + 20, (int)msgBox.y + 15, msgFontSize, WHITE);

		//Draw Retry Button - used to draw buttons once the timer has ended
		Rectangle restartBtn = { 470, 600, 260,50 };
		bool hovered = CheckCollisionPointRec(GetMousePosition(), restartBtn);
		Color btnColor = hovered ? DARKGRAY : RED;
		DrawRectangleRec(restartBtn, btnColor);


		const char* btnText = "Retry Last Checkpoint";
		int btnTextWidth = MeasureText(btnText, 20);

		//Centre and draw the label
		DrawText(
			btnText,
			(int)(restartBtn.x + (restartBtn.width - btnTextWidth) / 2.0f),
			(int)restartBtn.y + 15,
			20,
			WHITE
		);

	}
	
	//Used to draw buttons once timer has ended
	void Game::TimerRestart() {

		ClearBackground(RAYWHITE);
		Rectangle restartBtn = { GetScreenWidth() / 2 - 100, horizontalLineY + 80, 200, 40 };
		bool hovered = CheckCollisionPointRec(GetMousePosition(), restartBtn);
		DrawRectangleRec(restartBtn, hovered ? DARKGRAY : GRAY);
		DrawText("Retry last question", restartBtn.x + 30, restartBtn.y + 10, 20, WHITE);

	}

	//calls DrawRestartPrompt()
	//appears when the player fails the pathway and is given the choice to restart it
	void Game::DrawOptionRestartScreen()
	{
		DrawRestartPrompt();
	}


	void Game::DrawProgress() const
	{
		//progress bar tracks main question progression
		mainProgressBar.Draw();
		
		//else draw the separate pathway progress bar
		if (inPathway) { 
			pathwayProgressBar.Draw();

		}

	}
} 