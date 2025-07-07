#include "raylib.h"
#include "Ch2Questions.h"
#include <vector>
#include <string>
#include "DialogueBox.hpp"
#include "ProgressBar.h"

using namespace std;

namespace Chapter2 {
	class Ch2Game {
	private:
		int currentQuestionIndex = 0;
		int score = 0;
		int selectedOption = -1;
		Texture2D heart;
		bool inPathway = false;
		int pathwayOrigin = 0;

		float horizontalLineY;
		float verticalLineX;

		//for prompts
		bool showingPrompt = false;
		string currentPrompt;
		string waitingNextQuestion;

		//for restart chapter & 3 wrong in a row
		int wrongPathwayCount = 0;
		int currentMainQuestion = -1;
		bool showingFailureScreen = false;
		bool showingChapterRestartScreen = false;

		//for zombie attack
		bool zombieAttackCompleted;
		int requiredPresses;
		float timeLimit;
		Texture2D backgroundTexture, Ch2Background;

		//for lock-picking
		Texture2D buildingTexture;
		bool showingLockpickPrompt;
		bool showingLockpickGame;
		static const int SEQUENCE_LENGTH = 5;

		DialogueBox Ch2Dbox;

		bool isDead = false;
		bool chapterOver = false;

		// progress bars
		ProgressBar<vector<Question>> mainProgressBar;
		ProgressBar<vector<Question>> pathwayProgressBar;
		bool showingPathwayProgress = false;

		// question containers for main/pathway question
		vector<Question> mainQuestions;
		vector<Question> pathwayQuestions;



	public:
		Ch2Game();
		void Run();
		void DrawQuestionScreen();

		void DrawGameOverScreen();

		const vector<Question>& GetCurrentQuestions() const;
		pair<bool, int> ParseQuestionIndex(const string& next, int currentMainIndex);
		void ProcessAnswer(int selectedOption);
		void DrawHealth(int health);
		//for prompts
		//void ShowPrompt(const string& prompt);
		void DrawPromptScreen();
		void nextQuestion();

		//for restart chapter & 3 wrong in a row
		void DrawFailureScreen();
		void DrawChapterRestartScreen();

		//zombie attack spam
		void ZombieAttackChallenge();

		//for lock picking
		const char* KeyToString(KeyboardKey key);
		vector<KeyboardKey> GenerateKeySequence();
		void DrawLockpickPrompt();
		bool PlayLockPickingMiniGame();

		// Progress bar
		void DrawProgress() const;

		void SetDboxImage();
		bool isChapterOver();

		

		

	};
}