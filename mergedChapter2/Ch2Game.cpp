#include "Ch2Game.h"
#include "Ch2Questions.h"
#include <iostream>
#include "raylib.h" 
#include "DialogueBox.hpp"
#include "ProgressBar.h"

#include <random> 

#define MAX_HEALTH 5
int playerHealth = MAX_HEALTH;
using namespace std;

//for chapter start screen include you can hear zombies in text before going into the zombie attack 
namespace Chapter2 {
	Ch2Game::Ch2Game() : currentQuestionIndex(0), score(0), selectedOption(-1),
		zombieAttackCompleted(false), requiredPresses(15), timeLimit(5.0f), showingLockpickPrompt(false), showingLockpickGame(false),
		mainProgressBar(0, 770, 1205, 30, LIGHTGRAY, DARKBLUE, Questions::mainQuestions, "PROGRESS..."),
		pathwayProgressBar(0, 770, 1200, 30, LIGHTGRAY, RED, Questions::pathwayQuestions, "PROGRESS...") {
			//InitWindow(1200, 800, "Chapter 2");
			SetTargetFPS(60);

			horizontalLineY = GetScreenHeight() / 3.0f;
			verticalLineX = GetScreenWidth() * 0.75f;
			DialogueBox DBox("", "", 10);
			Ch2Dbox = DBox;
			heart = LoadTexture("Image_Resources/heart.png");
			backgroundTexture = LoadTexture("Image_Resources/zombieconvert.png");
			buildingTexture = LoadTexture("Image_Resources/buildingpixel.png");

			Image img = LoadImage("Image_Resources/chapter2Background.png");
			Ch2Background = LoadTextureFromImage(img);
		
	}

	void Ch2Game::Run() {

		DrawTexture(Ch2Background, 0, 0, WHITE);

		if (isDead || playerHealth <= 0) {
			DrawChapterRestartScreen(); 
		}

		if (!zombieAttackCompleted) {
			ZombieAttackChallenge();
		}

		if (zombieAttackCompleted && !showingChapterRestartScreen) {
			DrawTexturePro(
				Ch2Background,
				Rectangle{ 0, 0, (float)buildingTexture.width, (float)buildingTexture.height },
				Rectangle{ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
				Vector2{ 0, 0 },
				0.0f,
				WHITE
			);

			DrawHealth(playerHealth);

			if (showingLockpickPrompt) {
				DrawLockpickPrompt();
			}
			else if (showingLockpickGame) {
				bool success = PlayLockPickingMiniGame();
				if (success) {
					showingLockpickGame = false;
					showingPrompt = false;
					nextQuestion();
					Ch2Dbox.NextPage();
				}
				else {
					showingLockpickPrompt = true;
				}
			}
			else if (showingChapterRestartScreen || isDead) {
				DrawChapterRestartScreen();
			}
			else if (showingFailureScreen) {
				DrawFailureScreen();
			}
			else if (showingPrompt) {
				DrawProgress();
				DrawPromptScreen();
				Rectangle continueBtn = { GetScreenWidth() - 400, 340, 200, 40 };
				bool hovered = CheckCollisionPointRec(GetMousePosition(), continueBtn);

				if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					showingPrompt = false;
					nextQuestion();
					Ch2Dbox.NextPage();
				}
			}
			else if (currentQuestionIndex >= 0 && currentQuestionIndex < GetCurrentQuestions().size()) {
				DrawProgress();
				DrawQuestionScreen();
				

				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					Vector2 mousePos = GetMousePosition();
					const auto& currentQuestion = GetCurrentQuestions()[currentQuestionIndex];

					for (int i = 0; i < currentQuestion.options.size(); i++) {
						float yPos = horizontalLineY + 50 + i * 60;
						Rectangle optionRect = { 250, yPos + 100, 700, 30 };
						if (CheckCollisionPointRec(mousePos, optionRect)) {
							ProcessAnswer(i);
							break;
						}
					}
				}
			}
			else if (playerHealth > 0 && !isDead) {
				chapterOver = true;
			}

		}

	}

	void Ch2Game::SetDboxImage()
	{
		Ch2Dbox.setImage("Image_Resources/Sarah.png");
		Ch2Dbox.setBoxImage("Image_Resources/RedTextBox.png");
	}

	void Ch2Game::DrawHealth(int health) {

		for (int i = 0; i < health; i++) {
			Vector2 position = { 20 + i * 30, 340 };
			DrawTextureEx(heart, position, 0.0f, 0.025f, WHITE);
		}

	}

	const vector<Question>& Ch2Game::GetCurrentQuestions() const {
		return  inPathway ? Questions::pathwayQuestions : Questions::mainQuestions;
	}

	pair<bool, int> Ch2Game::ParseQuestionIndex(const string& next, int currentMainIndex)
	{
		if (next.empty() || next == "-1") {
			return make_pair(false, -1);
		}

		if (next.empty() || next == "-2") {
			showingChapterRestartScreen = true;
			isDead = true;
			chapterOver = false; 
			return make_pair(false, -2);
		}

		if (next == "1-B") return make_pair(true, 0);
		if (next == "2-B") return make_pair(true, 1);
		if (next == "2-C") return make_pair(true, 2);
		if (next == "4-B") return make_pair(true, 3);
		if (next == "4-C") return make_pair(true, 4);
		if (next == "5-B") return make_pair(true, 5);
		if (next == "5-C") return make_pair(true, 6);
		if (next == "6-B") return make_pair(true, 7);
		if (next == "6-C") return make_pair(true, 8);
		if (next == "7-B") return make_pair(true, 9);
		if (next == "7-C") return make_pair(true, 10);

		return make_pair(false, atoi(next.c_str()) - 1);
	}

	void Ch2Game::ProcessAnswer(int selectedOption) {
		this->selectedOption = selectedOption;
		const auto& currentQuestion = GetCurrentQuestions()[currentQuestionIndex];

		playerHealth += currentQuestion.outcomes[selectedOption];
		playerHealth = max(0, min(playerHealth, MAX_HEALTH));

		if (playerHealth <= 0) {
			showingChapterRestartScreen = true;
			return;
		}
		else
		{

		}


		//for 3 wrong
		if (inPathway) {
			if (currentQuestion.outcomes[selectedOption] < 0) {
				wrongPathwayCount++;

				if (wrongPathwayCount >= 2 && currentMainQuestion != -1) {
					showingFailureScreen = true;
					return;
				}
			}
			else {
				wrongPathwayCount = 0;
			}
		}
		else {
			if (currentQuestionIndex >= 1 && currentQuestionIndex <= 6) {
				currentMainQuestion = currentQuestionIndex;
			}
			else {
				currentMainQuestion = -1;
			}
		}




		//for prompt
		waitingNextQuestion = currentQuestion.nextQuestions[selectedOption];
		currentPrompt = currentQuestion.prompts[selectedOption];
		showingPrompt = true;

		/*if (playerHealth <= 0) {
			waitingNextQuestion = "-1";
		} */

	}
	void Ch2Game::DrawPromptScreen()
	{
		DrawHealth(playerHealth);
		Ch2Dbox.setText(currentPrompt.c_str());
		Ch2Dbox.DrawDialogueBox(27, 30);
		//Ch2Dbox.NextPage();

		Rectangle continueBtn = { GetScreenWidth() - 400, 340, 200, 40 };
		bool hovered = CheckCollisionPointRec(GetMousePosition(), continueBtn);
		int contCent = continueBtn.x + (200 - MeasureText("Continue", 20)) / 2;

		DrawRectangleRec(continueBtn, hovered ? DARKGRAY : BLACK);
		DrawText("Continue", contCent, continueBtn.y + 10, 20, WHITE);

		Ch2Dbox.NextPage();
	}
	//moved from processAnswer for finding next question
	void Ch2Game::nextQuestion()
	{
		if (showingFailureScreen || showingChapterRestartScreen) {
			return; //player has to press button to coninue 
		}

		//if (currentQuestionIndex == 3 && !showingLockpickPrompt && !showingLockpickGame && !inPathway) {
		//    showingLockpickPrompt = true;
		//    //currentPrompt = "The door to the shelter is locked. You'll need to pick it to proceed.";
		//    return;
		//}

		if (!showingPrompt) {
			if (waitingNextQuestion.empty()) {
				currentQuestionIndex++;
				if (currentQuestionIndex >= GetCurrentQuestions().size()) {
					currentQuestionIndex = -1;
				}
			}
			else {
				if (waitingNextQuestion == "-1") {
					currentQuestionIndex = -1;
					return;
				}

				pair<bool, int> result = ParseQuestionIndex(
					waitingNextQuestion,
					inPathway ? pathwayOrigin : currentQuestionIndex
				);

				if (result.first) {
					// Pathway question
					pathwayProgressBar += 1; 
					pathwayOrigin = currentQuestionIndex;
					currentQuestionIndex = result.second;
					inPathway = true;
				}
				else {
					// Main question
					mainProgressBar += 1; 
					currentQuestionIndex = result.second;
					if (currentQuestionIndex == 4 && !showingLockpickPrompt && !showingLockpickGame) {
						showingLockpickPrompt = true;
						currentQuestionIndex--;
					}
					inPathway = false;
					if (currentQuestionIndex < 1 || currentQuestionIndex > 6) {
						wrongPathwayCount = 0;
					}
				}
				//mainProgressBar.Update(currentQuestionIndex);
				
				waitingNextQuestion.clear();
			}
		}

		selectedOption = -1;

		if (playerHealth <= 0) {
			currentQuestionIndex = -1;
		}
	}

	//if 3 wrong questions
	void Ch2Game::DrawFailureScreen()
	{

		DrawTexturePro(
			Ch2Background,
			Rectangle{ 0, 0, (float)buildingTexture.width, (float)buildingTexture.height },
			Rectangle{ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
			Vector2{ 0, 0 },
			0.0f,
			WHITE
		);

		DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{ 0, 0, 0, 150 });

		const char* title = "CHAPTER 2: THE RUINS";
		int titleFontSize = 60;
		float titleWidth = (float)MeasureText(title, titleFontSize);

		Rectangle titleBox = {
			(GetScreenWidth() - titleWidth - 40.0f) / 2.0f,
			120.0f,
			titleWidth + 40.0f,
			100.0f
		};


		DrawRectangleRec(titleBox, Color{ 0, 0, 0, 200 });
		DrawRectangleLinesEx(titleBox, 3.0f, WHITE);
		DrawText(title, (int)titleBox.x + 20, (int)titleBox.y + 30, titleFontSize, WHITE);

		const char* message = "You failed this question by getting the last 3 wrong";

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

		Rectangle restartBtn = { 470, 600, 260,50 };
		

		bool hovered = CheckCollisionPointRec(GetMousePosition(), restartBtn);
		DrawRectangleRec(restartBtn, hovered ? DARKGRAY : boxColor);

		const char* btnText = "RETRY CHECKPOINT";
		int btnTextWidth = MeasureText(btnText, 20);
		DrawText(btnText,
			(int)(restartBtn.x + (restartBtn.width - btnTextWidth) / 2.0f),
			(int)restartBtn.y + 15,
			20,
			WHITE);

		if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			currentQuestionIndex = currentMainQuestion;
			inPathway = false;
			wrongPathwayCount = 0;
			showingFailureScreen = false;
			playerHealth += 3;
			selectedOption = -1;

			waitingNextQuestion.clear();
			showingPrompt = false;
			currentPrompt.clear();

		}


	}

	//if lost all health
	void Ch2Game::DrawChapterRestartScreen()
	{
		bool isStart = (currentQuestionIndex == 0 && playerHealth == 5);

		DrawTexturePro(
			Ch2Background,
			Rectangle{ 0, 0, (float)buildingTexture.width, (float)buildingTexture.height },
			Rectangle{ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
			Vector2{ 0, 0 },
			0.0f,
			WHITE
		);

		DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{ 0, 0, 0, 150 });

		const char* title = "CHAPTER 2: THE RUINS";
		int titleFontSize = 60;
		float titleWidth = (float)MeasureText(title, titleFontSize);

		Rectangle titleBox = {
			(GetScreenWidth() - titleWidth - 40.0f) / 2.0f,
			120.0f,
			titleWidth + 40.0f,
			100.0f
		};


		DrawRectangleRec(titleBox, Color{ 0, 0, 0, 200 });
		DrawRectangleLinesEx(titleBox, 3.0f, WHITE);
		DrawText(title, (int)titleBox.x + 20, (int)titleBox.y + 30, titleFontSize, WHITE);

		const char* message;

		if (isStart) {
			message = "You lose a life for every question you get wrong";
		}
		else {
			if (isDead) {
				message = "You both died";
			}
			else
			{
				message = "You lost all 5 lives!";
			}


		}
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

		Rectangle restartBtn = { 470, 600, 260,50 };

		bool hovered = CheckCollisionPointRec(GetMousePosition(), restartBtn);
		DrawRectangleRec(restartBtn, hovered ? DARKGRAY : boxColor);

		const char* btnText;

		if (isStart) {
			btnText = "START CHAPTER";
		}
		else {
			btnText = "RESTART CHAPTER";

		}

		int btnTextWidth = MeasureText(btnText, 20);
		DrawText(btnText,
			(int)(restartBtn.x + (restartBtn.width - btnTextWidth) / 2.0f),
			(int)restartBtn.y + 15,
			20,
			WHITE);

		if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			currentQuestionIndex = 0;
			inPathway = false;
			wrongPathwayCount = 0;
			currentMainQuestion = -1;
			showingChapterRestartScreen = false;
			playerHealth = MAX_HEALTH;
			selectedOption = -1;
			isDead = false; 
			mainProgressBar -= mainProgressBar.GetCurrentIndex();
			pathwayProgressBar -= pathwayProgressBar.GetCurrentIndex();
		}

	}

	void Ch2Game::ZombieAttackChallenge()
	{
		int pressCount = 0;
		float startTime = GetTime();
		bool attackStarted = false;
		bool gameOver = false;

		while (!WindowShouldClose() && !zombieAttackCompleted) {
			BeginDrawing();
			DrawTexturePro(
				backgroundTexture,
				Rectangle{ 0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height },
				Rectangle{ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
				Vector2{ 0, 0 },
				0.0f,
				WHITE
			);

			if (!attackStarted && !gameOver) {
				DrawRectangle(GetScreenWidth() / 2 - 250, GetScreenHeight() / 2 - 100, 500, 200, Color{ 0, 0, 0, 200 });
				int centSpam = (GetScreenWidth() / 2 - 250) + (500 - MeasureText("SPAM SPACE rapidly to survive!", 20)) / 2;
				int centWarning = (GetScreenWidth() / 2 - 250) + (500 - MeasureText("ZOMBIE ATTACK INCOMING!!", 30)) / 2;
				DrawText("ZOMBIE ATTACK INCOMING!!", centWarning, GetScreenHeight() / 2 - 70, 30, RED);
				DrawText("SPAM SPACE rapidly to survive!", centSpam, GetScreenHeight() / 2 - 30, 20, WHITE);

				Rectangle startBtn = { GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 + 30, 200, 50 };
				bool hovered = CheckCollisionPointRec(GetMousePosition(), startBtn);

				DrawRectangleRec(startBtn, hovered ? DARKGRAY : RED);
				int cent = 500 + (startBtn.width - MeasureText("BEGIN ATTACK", 20)) / 2;
				DrawText("BEGIN ATTACK", cent, startBtn.y + 15, 20, WHITE);

				if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					attackStarted = true;
					startTime = GetTime();
				}
			}
			else if (gameOver) {
				DrawRectangle(GetScreenWidth() / 2 - 250, GetScreenHeight() / 2 - 100, 500, 200, Color{ 0, 0, 0, 200 });
				DrawText("YOU DIED!", (500-MeasureText("YOU DIED!",40))/2 + 350, GetScreenHeight() / 2 - 70, 40, RED);
				DrawText("Press SPACE to try again", (500 - MeasureText("Press SPACE to try again", 20)) / 2 +350, GetScreenHeight() / 2 - 20, 20, WHITE);

				if (IsKeyPressed(KEY_SPACE)) {
					gameOver = false;
					attackStarted = false;
					playerHealth = MAX_HEALTH;
					pressCount = 0;
				}
			}
			else {
				DrawRectangle(200, 50, 800, 200, Color{ 0, 0, 0, 150 });
				float currentTime = GetTime();
				float elapsed = currentTime - startTime;


				if (playerHealth > 0) {
					if (IsKeyPressed(KEY_SPACE)) {
						pressCount++;
					}

					if (pressCount >= requiredPresses) {
						zombieAttackCompleted = true;
					}

					if (elapsed >= timeLimit) {
						playerHealth--;
						startTime = GetTime();
						pressCount = 0;
					}
				}


				if (playerHealth <= 0) {
					gameOver = true;
				}

				DrawRectangle(200, 50, 800, 200, Color{ 0, 0, 0, 150 });

				int centAttack = 200 + (800 - MeasureText("ZOMBIE ATTACK!!!!", 30)) / 2;
				int centSpamAgain = 200 + (800 - MeasureText("SPAM SPACE to survive!", 20)) / 2;
				DrawText("ZOMBIE ATTACK!!!!", centAttack, 80, 30, RED);
				DrawText("SPAM SPACE to survive!", centSpamAgain, 130, 20, LIGHTGRAY);

				if (!gameOver) {

					int centPress = 200 + (800 - MeasureText(TextFormat("Presses: %d / %d", pressCount, requiredPresses), 20)) / 2;
					int centTime = 200 + (800 - MeasureText(TextFormat("Time Left: %.1f", timeLimit - elapsed), 20)) / 2;

					DrawText(TextFormat("Presses: %d / %d", pressCount, requiredPresses), centPress, 180, 20, GREEN);
					DrawText(TextFormat("Time Left: %.1f", timeLimit - elapsed), centTime, 220, 20, YELLOW);
				}
			}

			DrawHealth(playerHealth);
			EndDrawing();
		}

	}

	const char* Ch2Game::KeyToString(KeyboardKey key)
	{
		switch (key) {
		case KEY_A: return "A";
		case KEY_S: return "S";
		case KEY_D: return "D";
		case KEY_W: return "W";
		default: return "Unknown";
		}
	}

	vector<KeyboardKey> Ch2Game::GenerateKeySequence()
	{
		vector<KeyboardKey> keys = { KEY_A, KEY_S, KEY_D, KEY_W };
		vector<KeyboardKey> sequence;

		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> distrib(0, keys.size() - 1);

		for (int i = 0; i < SEQUENCE_LENGTH; i++) {
			int randomIndex = distrib(gen);
			sequence.push_back(keys[randomIndex]);
		}

		return sequence;
	}

	void Ch2Game::DrawLockpickPrompt()
	{
		DrawTexturePro(
			buildingTexture,
			Rectangle{ 0, 0, (float)buildingTexture.width, (float)buildingTexture.height },
			Rectangle{ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
			Vector2{ 0, 0 },
			0.0f,
			WHITE
		);

		const char* line1 = "Jacob's shelter is an old office building,";
		const char* line2 = "and its doors are securely locked.";
		const char* line3 = "You'll need to pick the lock to proceed.";

		int textWidth1 = MeasureText(line1, 30);
		int textWidth2 = MeasureText(line2, 30);
		int textWidth3 = MeasureText(line3, 30);

		int maxTextWidth = max(max(textWidth1, textWidth2), textWidth3);
		int boxWidth = maxTextWidth + 40;
		int boxHeight = 180;

		int boxX = GetScreenWidth() / 2 - boxWidth / 2;
		int boxY = GetScreenHeight() / 2 - boxHeight / 2 - 20;

		DrawRectangle(boxX, boxY, boxWidth, boxHeight, Color{ 0, 0, 0, 200 });

		DrawText(line1, GetScreenWidth() / 2 - textWidth1 / 2, boxY + 30, 30, WHITE);
		DrawText(line2, GetScreenWidth() / 2 - textWidth2 / 2, boxY + 70, 30, WHITE);
		DrawText(line3, GetScreenWidth() / 2 - textWidth3 / 2, boxY + 110, 30, WHITE);

		Rectangle startBtn = { 450, boxY + boxHeight + 20, 300, 50 };
		bool hovered = CheckCollisionPointRec(GetMousePosition(), startBtn);

		DrawRectangleRec(startBtn, hovered ? DARKGRAY : BLACK);
		int cent = 450 + (startBtn.width - MeasureText("ATTEMPT LOCK-PICK", 20)) / 2;
		DrawText("ATTEMPT LOCK-PICK", cent, startBtn.y + 15, 20, RED);

		if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			showingLockpickPrompt = false;
			showingLockpickGame = true;
		}
	}

	bool Ch2Game::PlayLockPickingMiniGame()
	{
		vector<KeyboardKey> sequence = GenerateKeySequence();
		int currentIndex = 0;

		bool showResult = false;
		bool result = false;
		float resultTimer = 0;
		const float RESULT_DISPLAY_TIME = 1.5f;

		while (!WindowShouldClose() && !showResult) {
			BeginDrawing();
			DrawTexturePro(
				buildingTexture,
				Rectangle{ 0, 0, (float)buildingTexture.width, (float)buildingTexture.height },
				Rectangle{ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
				Vector2{ 0, 0 },
				0.0f,
				WHITE
			);

			DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{ 0, 0, 0, 180 });

			DrawText("LOCKPICKING SEQUENCE",
				GetScreenWidth() / 2 - MeasureText("LOCKPICKING SEQUENCE", 30) / 2,
				50, 30, WHITE);

			DrawText("Press the correct keys in order to unlock",
				(GetScreenWidth() - MeasureText("Press the correct keys in order to unlock", 20)) / 2,
				90, 20, LIGHTGRAY);

			int x = GetScreenWidth() / 2 - (SEQUENCE_LENGTH * 60) / 2;
			for (int i = 0; i < sequence.size(); i++) {
				const char* label = KeyToString(sequence[i]);
				Color color = (i < currentIndex) ? GREEN : RAYWHITE;
				DrawText(label, x + i * 60, 150, 40, color);
			}

			EndDrawing();


			if (IsKeyPressed(sequence[currentIndex])) {
				currentIndex++;
				if (currentIndex >= sequence.size()) {
					result = true;
					showResult = true;
				}
			}
			else {
				bool wrongKeyPressed = false;
				for (int k = KEY_A; k <= KEY_Z; k++) {
					if (IsKeyPressed((KeyboardKey)k)) {
						wrongKeyPressed = true;
						break;
					}
				}

				if (wrongKeyPressed) {
					result = false;
					showResult = true;
				}
			}
		}
		if (showResult) {
			float startTime = GetTime();
			while (GetTime() - startTime < RESULT_DISPLAY_TIME && !WindowShouldClose()) {
				BeginDrawing();

				DrawTexturePro(
					buildingTexture,
					Rectangle{ 0, 0, (float)buildingTexture.width, (float)buildingTexture.height },
					Rectangle{ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
					Vector2{ 0, 0 },
					0.0f,
					WHITE
				);

				DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{ 0, 0, 0, 180 });

				if (result) {
					DrawText("Unlocked successfully!",
						GetScreenWidth() / 2 - MeasureText("Unlocked successfully!", 40) / 2,
						GetScreenHeight() / 2 - 20, 40, GREEN);
				}
				else {
					DrawText("You failed to pick the lock! Try again!",
						GetScreenWidth() / 2 - MeasureText("You failed to pick the lock! Try again!", 40) / 2,
						GetScreenHeight() / 2 - 20, 40, RED);
				}

				EndDrawing();
			}
		}

		return result;
	}

	void Ch2Game::DrawQuestionScreen() {
		selectedOption = -1;
		const auto& currentQuestions = GetCurrentQuestions();

		if (currentQuestionIndex < 0 || currentQuestionIndex >= currentQuestions.size()) {
			return;
		}

		const auto& currentQuestion = currentQuestions[currentQuestionIndex];

		Ch2Dbox.setText(currentQuestion.text.c_str());
		Ch2Dbox.DrawDialogueBox(27, 30);
		DrawHealth(playerHealth);



		for (int i = 0; i < currentQuestion.options.size(); i++) {
			float yPos = horizontalLineY + 40 + i * 60;

			bool hovered = CheckCollisionPointRec(GetMousePosition(), Rectangle{ 235, yPos + 100, 730, 30 });
			DrawRectangleRec(Rectangle{ 235, yPos + 110, 730, 30 }, hovered ? RED : ColorFromHSV(0, 1, 0.4));

			int centText = 235 + (730 - MeasureText(currentQuestion.options[i].c_str(), 20)) / 2;


			DrawText(currentQuestion.options[i].c_str(), centText, yPos + 115, 20, WHITE);

		}
		Ch2Dbox.NextPage();
	}

	void Ch2Game::DrawGameOverScreen()
	{
		ClearBackground(RAYWHITE);
		DrawText("End of Chapter 2", GetScreenWidth() / 2 - 150, GetScreenHeight() / 2 - 40, 40, RED);
		DrawText(TextFormat("Final Health: %i/%i", playerHealth, MAX_HEALTH),
			GetScreenWidth() / 2 - 100, GetScreenHeight() / 2, 30, BLACK);
	}

	bool Ch2Game::isChapterOver()
	{
		if (chapterOver && !isDead)
		{
			return chapterOver;
		}
	}

	void Ch2Game::DrawProgress() const
	{
		mainProgressBar.Draw();
		if (inPathway) {
			pathwayProgressBar.Draw();
		}

	}

}