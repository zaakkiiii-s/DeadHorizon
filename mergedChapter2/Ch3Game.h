
#include "raylib.h"
#include "Ch3Question.h"
#include "timer.h"
#include <vector>
#include "DialogueBox.hpp"
#include "ProgressBar.h"


namespace Chapter3 {
	class Game {
	private:
		Timer questionTimer;
		int currentQuestionIndex = 0;
		int score = 0;
		int selectedOption = -1;

		bool timerExpired;
		bool enteredPathway = false;
		bool inPathway = false;
		int pathwayOrigin = 0;
		bool justEnteredPathway = false;
		bool isGameOver = false;

		float horizontalLineY;
		float verticalLineX;

		//for prompts
		bool showingPrompt = false;
		string currentPrompt;
		string waitingNextQuestion;
		bool isStart = true;
		bool showingChapterRestartScreen = false;
		bool showTimeUpScreen = false;

		DialogueBox Ch3Dbox;
		Texture2D Ch3Background1, Ch3Background2;

		//connect wires
		bool showingConnectWiresPrompt;
		bool showingConnectWiresGame;
		Texture2D miniBackground;
		Texture2D wiresBackground;
		Texture2D redSocket;
		Texture2D blueSocket;
		Texture2D yellowSocket;

		//Camo Variables
		bool showingFindCamoPrompt = false ; 
		bool showingFindCamoGame = false; 
		Texture2D background; 
		Texture2D camo; 
		int timerLimit = 15;
		float gameTimer = timerLimit;
		bool gameOver = false;
		bool foundCamo = false;
		double lastRestartTime = 0;
		int camoWidthScaled = 110;
		int camoHeightScaled = 110;
		Vector2 camoPos = { 0, 0 };

		// progress bars
		ProgressBar<vector<Question>> mainProgressBar;
		ProgressBar<vector<Question>> pathwayProgressBar;
		bool showingPathwayProgress = false;

		// question containers for main/pathway question
		vector<Question> mainQuestions;
		vector<Question> pathwayQuestions;

		//EndingMovement
		bool showingEndingPrompt = false; 
		bool showingEndingMove = false; 
		Texture2D characterSheet; 
		Texture2D pathway; 

		
		int currentFrame = 0;
		float frameTimer = 0.0f;

		// Game state
		bool canMove = true;
		bool cureDelivered = false;
		bool timer = false;
		float timerStart = 0.0f;
		bool gameOverEnd = false;
		bool Excute = false;
		bool chapterOver = false;
		


	public:
		Game();
		void Run();
		void DrawQuestionScreen();

		void DrawRestartPrompt();
		void TimerRestart();
		void DrawGameOverScreen();
		void DrawTimeUpScreen();

		void DrawOptionRestartScreen();

		const vector<Question>& GetCurrentQuestions() const;
		pair<bool, int> ParseQuestionIndex(const string& next, int currentMainIndex);
		void ProcessAnswer(int selectedOption);

		//for prompts
		void DrawPromptScreen();
		void nextQuestion();
		void DrawChapterStartScreen();

		void setDboxImage();

		//Connect Game
		struct Wire {
			Rectangle rect;
			Vector2 offset;
			bool dragging = false;
			bool placed = false;
			Color color;
		};

		//connect game
		void DrawConnectWiresPrompt(); 
		bool PlayConnectWiresGame();  
		bool CheckWirePlace(Wire& wire, Rectangle target);
		void Reset(float& timer, bool& gameWon, bool& gameOver, vector<Wire>& wires);
		//finding camo 
		void DrawCamoPrompt(); 
		bool PlayCamoGame(); 

		// Progress bar
		void DrawProgress() const;

		//draw ending 
		void DrawEndingPrompt();
		bool EndingMovement(); 
		bool isChapterOver(); 

};
}