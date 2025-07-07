#include "DialogueBox.hpp"
#include "raylib.h"
#include "Questions.h"
#include <vector>
#include "ProgressBar.h"

namespace ZombieGame {
	class Game {
	private:
		int currentQuestionIndex = 0;
		int score = 0;
		int selectedOption = -1;

		bool inPathway = false;
		int pathwayOrigin = 0;

		float horizontalLineY;
		float verticalLineX;

		//for prompts
		bool showingPrompt = false;
		bool nextChapter = false;
		string currentPrompt;
		string waitingNextQuestion;

		Texture2D texture;
		DialogueBox Ch1Dbox;
		bool showingChapterScreen = false; 

		// progress bars
		ProgressBar<vector<Question>> mainProgressBar;
		ProgressBar<vector<Question>> pathwayProgressBar;
		bool showingPathwayProgress = false;

		// question containers for main/pathway question
		vector<Question> mainQuestions;
		vector<Question> pathwayQuestions;


	public:
		Game();
		void Run();
		void DrawQuestionScreen();

		void DrawGameOverScreen();

		const vector<Question>& GetCurrentQuestions() const;
		pair<bool, int> ParseQuestionIndex(const string& next, int currentMainIndex);
		void ProcessAnswer(int selectedOption);

		//for prompts
		void ShowPrompt(const string& prompt);
		void DrawPromptScreen();
		void nextQuestion();

		void setDboxImage();
		bool chapterOver();

		void drawChapter1Restart();
		bool shouldRestart() const {
			return (currentQuestionIndex >= GetCurrentQuestions().size() && score < 10);
		}
		bool DrawPauseButton();

		// Progress bar
		void UpdateProgress(int newIndex, bool isPathway);
		void DrawProgress() const;
	
	};
}