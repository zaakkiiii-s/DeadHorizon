#include "Game.h"
#include "DialogueBox.hpp"
#include "Questions.h"
#include <iostream>
#include "raylib.h"
#include "ProgressBar.h"

using namespace std;

namespace ZombieGame {
	//Constructor intialised
	Game::Game() : currentQuestionIndex(0), score(0), selectedOption(-1),
		//Initialize progress bars for main and pathway
		mainProgressBar(0, 770, 1205, 30, LIGHTGRAY, DARKGREEN, Questions::mainQuestions, "PROGRESS..."),
		pathwayProgressBar(0, 770, 12005, 30, LIGHTGRAY, RED, Questions::pathwayQuestions, "PROGRESS...") {
		
		//Create window
		InitWindow(1200, 800, "Dead Horizon");
		SetTargetFPS(60);	//frame rate	

		Image ch1Image = LoadImage("Image_Resources/Chapter1_Background.png");
		texture = LoadTextureFromImage(ch1Image);

		horizontalLineY = GetScreenHeight() / 3.0f;
		verticalLineX = GetScreenWidth() * 0.75f;

		//Initialize dialogue box
		DialogueBox DBox("", "", 10);
		Ch1Dbox = DBox;
	}

	void Game::setDboxImage()
	{
		//Set the main image
		Ch1Dbox.setImage("Image_Resources/1.jpg");
		Ch1Dbox.setBoxImage("Image_Resources/AITextBox.png");	//Set the background
	}

	void Game::Run() {

		DrawTexture(texture, 0, 0, WHITE);
		//if boolean is true show prompt
		if (showingPrompt) {
			DrawPromptScreen();
			DrawProgress();
			//Draw continue button
			Rectangle continueBtn = { GetScreenWidth() - 400, 330, 200, 40 };
			bool hovered = CheckCollisionPointRec(GetMousePosition(), continueBtn);

			if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				showingPrompt = false;
				nextQuestion();
			}
		}
		//Main question screen logic
		else if (currentQuestionIndex < GetCurrentQuestions().size()) {
			DrawQuestionScreen();
			DrawProgress(); 

			//
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				Vector2 mousePos = GetMousePosition();
				const auto& currentQuestion = GetCurrentQuestions()[currentQuestionIndex];

				for (int i = 0; i < currentQuestion.options.size(); i++) {
					float yPos = horizontalLineY + 50 + i * 60;
					Rectangle optionRect = { 350, yPos + 100, 500, 30 };
					//Process function is called when user selects an answer
					if (CheckCollisionPointRec(mousePos, optionRect)) {
						ProcessAnswer(i);
						break;
					}
				}
			}
		}
		//Move to next chapter if condition met
		else if (score >= 10) {
			nextChapter = true;
		}
		//restart chapter if <10
		else if (score < 10) {
			drawChapter1Restart();
			return;
		}

	}

	//return pathway questions if player is answering sub questions else return main questions
	const vector<Question>& Game::GetCurrentQuestions() const {
		return inPathway ? Questions::pathwayQuestions : Questions::mainQuestions;
	}

	//If the input string is empty, return false
	pair<bool, int> Game::ParseQuestionIndex(const string& next, int currentMainIndex) {
		if (next.empty()) {
			return make_pair(false, -1);
		}
		//mapping strings to pathway indices
		if (next == "1-B") return make_pair(true, 0);
		if (next == "1-C") return make_pair(true, 1);
		if (next == "2-B") return make_pair(true, 2);
		if (next == "3-B") return make_pair(true, 3);
		if (next == "4-B") return make_pair(true, 4);

		//Return as main question index
		return make_pair(false, atoi(next.c_str()) - 1);
	}

	void Game::ProcessAnswer(int selectedOption) {
		//store selected answer 
		this->selectedOption = selectedOption;
		const auto& currentQuestion = GetCurrentQuestions()[currentQuestionIndex];	//get current question

		score += currentQuestion.outcomes[selectedOption];

		if (score <= 0) {
			score = 0;
		}

	
		waitingNextQuestion = currentQuestion.nextQuestions[selectedOption];

		//prompt text shown on dialogue box
		currentPrompt = currentQuestion.prompts[selectedOption];
		showingPrompt = true;
	}

	void Game::DrawPromptScreen()
	{	
		//Set the dialogue box text to the current prompt
		Ch1Dbox.setText(currentPrompt.c_str());
		Ch1Dbox.DrawDialogueBox();
		Ch1Dbox.NextPage();

		//Draw continue button
		Rectangle continueBtn = { GetScreenWidth() - 400, 330, 200, 40 };
		bool hovered = CheckCollisionPointRec(GetMousePosition(), continueBtn);
		int contCent = continueBtn.x + (200 - MeasureText("Continue", 20)) / 2;

		DrawRectangleRec(continueBtn, hovered ? DARKGRAY : BLACK);
		DrawText("Continue", contCent, continueBtn.y + 10, 20, WHITE);
		DrawText(TextFormat("Score: %i", score), 1060, 60, 25, WHITE);	//Draw current score at the top right 
	}
	
	void Game::nextQuestion()
	{	
		//If no more questions end game
		if (waitingNextQuestion.empty()) {
			currentQuestionIndex = (int)GetCurrentQuestions().size(); 
			return;
		}

		// Parse the next question index based on the current game state
		pair<bool, int> result = ParseQuestionIndex(waitingNextQuestion, inPathway ? pathwayOrigin : currentQuestionIndex);

		//Handle pathway logic
		if (result.first) { // Pathway question
			pathwayProgressBar+=1;
			pathwayOrigin = currentQuestionIndex;
			currentQuestionIndex = result.second;
			inPathway = true;
			showingPathwayProgress = true;
		}
		else { // Main question
			mainProgressBar += 1;	//Increment main progress bar
			if (result.second == -1) {
				currentQuestionIndex = (int)GetCurrentQuestions().size(); // End game
			}
			else {
				currentQuestionIndex = result.second;
				inPathway = false;
				showingPathwayProgress = false;	//Hide pathway progress bar
			}
		}

		waitingNextQuestion.clear();
		selectedOption = -1;

	}

	void Game::DrawQuestionScreen() {
		const auto& currentQuestions = GetCurrentQuestions();

		//Prevent drawing if the current question index is invalid
		if (currentQuestionIndex < 0 || currentQuestionIndex >= currentQuestions.size()) {
			return;
		}

		const auto& currentQuestion = currentQuestions[currentQuestionIndex];

		//Display the question text
		Ch1Dbox.setText(currentQuestion.text.c_str());
		Ch1Dbox.DrawDialogueBox();
		DrawText(TextFormat("Score: %i", score), 1060, 60, 25, WHITE);

		//Loop through each available option for the current question
		for (int i = 0; i < currentQuestion.options.size(); i++) {
			float yPos = horizontalLineY + 50 + i * 60;


			//check hovering of text
			bool hovered = CheckCollisionPointRec(GetMousePosition(), Rectangle{ 350, yPos + 100, 500, 30 });
			DrawRectangleRec(Rectangle{ 350, yPos + 100, 500, 30 }, hovered ? DARKGRAY : ColorLerp(ColorFromHSV(122, 0.566, 0.3), ColorFromHSV(0, 0, 0.2), 0.25));

			int centText = (500 - MeasureText(currentQuestion.options[i].c_str(), 20)) / 2;
			DrawText(currentQuestion.options[i].c_str(), centText + 350, yPos + 105, 20, WHITE);
			Ch1Dbox.NextPage();


		}
	}


	// transition to next chapter
	bool Game::chapterOver()
	{
		if (nextChapter)
		{
			return true;
		}

	}
	void Game::drawChapter1Restart()
	{
		//Check if it's the start of the chapter
		bool isStart = (currentQuestionIndex == 0 && score == 0);


		DrawTexturePro(
			texture,
			Rectangle{ 0, 0, (float)texture.width, (float)texture.height },
			Rectangle{ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
			Vector2{ 0, 0 },
			0.0f,
			WHITE
		);

		DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{ 0, 0, 0, 150 });
		//Display Chapter title
		const char* title = "CHAPTER 1: THE AWAKENING";
		int titleFontSize = 60;
		float titleWidth = (float)MeasureText(title, titleFontSize);

		Rectangle titleBox = {
			(GetScreenWidth() - titleWidth - 40.0f) / 2.0f,
			120.0f,
			titleWidth + 40.0f,
			100.0f
		};

		//Draw title background and text
		DrawRectangleRec(titleBox, Color{ 0, 0, 0, 200 });
		DrawRectangleLinesEx(titleBox, 3.0f, WHITE);
		DrawText(title, (int)titleBox.x + 20, (int)titleBox.y + 30, titleFontSize, WHITE);

		const char* message;
		//Set the message text depending on game state
		if (isStart) {
			message = "You must score at least 10 points in this Chapter";
		}
		else {

			message = TextFormat("You scored %d/10: You need 10 to proceed to Chapter 2", score);

		}
		int msgFontSize = 30;
		float msgWidth = (float)MeasureText(message, msgFontSize);

		Rectangle msgBox = {
			(GetScreenWidth() - msgWidth - 40.0f) / 2.0f,
			280.0f,
			msgWidth + 40.0f,
			60.0f
		};

		Color boxColor = ColorLerp(ColorFromHSV(122.0f, 0.566f, 0.3f), ColorFromHSV(0.0f, 0.0f, 0.2f), 0.25f);
		DrawText(message, (int)msgBox.x + 20, (int)msgBox.y + 15, msgFontSize, WHITE);
		//Draw restart button
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

		//Reset all flags when restart button is pressed as well as go back to intial question
		if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			currentQuestionIndex = 0;
			score = 0;
			selectedOption = -1;
			inPathway = false;
			showingPrompt = false;
			nextChapter = false;
			waitingNextQuestion.clear();
			currentPrompt.clear();
			mainProgressBar -= mainProgressBar.GetCurrentIndex();  
			pathwayProgressBar -= pathwayProgressBar.GetCurrentIndex();
		}

	}

	bool Game::DrawPauseButton() {
		// Define button dimensions and position
		float buttonWidth = 90;
		float buttonHeight = 35;
		float padding = 10;
		Rectangle pauseBtn = { GetScreenWidth() - buttonWidth - (4 * padding), padding, buttonWidth, buttonHeight };

		Vector2 mouse = GetMousePosition();
		bool hovering = CheckCollisionPointRec(mouse, pauseBtn);
		bool clicked = hovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

		// Draw rounded rectangle with text
		DrawRectangleRounded(pauseBtn, 0.3f, 10, hovering ? LIGHTGRAY : GRAY);
		int fontSize = 18;
		int textWidth = MeasureText("Pause", fontSize);
		Vector2 textPos = { pauseBtn.x + (pauseBtn.width - textWidth) / 2, pauseBtn.y + (pauseBtn.height - fontSize) / 2 };
		DrawText("Pause", textPos.x, textPos.y, fontSize, BLACK);

		return clicked;
	}
	
	//Drawing of progress bar
	void Game::DrawProgress() const
	{
		mainProgressBar.Draw();

		if (showingPathwayProgress) {
			pathwayProgressBar.Draw();

			//x position and widgth of progress bar
			float pathwayX = pathwayProgressBar.GetX();
			float pathwayWidth = pathwayProgressBar.GetWidth();

			DrawText("Additional Pathway Questions",
				pathwayX + pathwayWidth / 2 - MeasureText("Additional Pathway Questions", 18) / 2,
				pathwayProgressBar.GetY() - 25,
				18, BLUE);
		}

	}
}
