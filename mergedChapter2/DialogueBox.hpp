#pragma once
#include "raylib.h"
#include <iostream>
#include <vector>
using namespace std;

class DialogueBox
{
private:
	int boxWidth, boxHeight;
	const char* text;
	Texture2D texture, boxTexture;
	int fontsize;
	int currentpage;
	vector<char*> textWall;

public:
	DialogueBox();
	DialogueBox(const char* text, const char* imagePath, int fontSize);
	DialogueBox(const char* text, const char* imagePath, const char* boxImage, int fontSize);
	~DialogueBox();
	void DrawDialogueBox();
	void DrawDialogueBox(int x, int y);
	void NextPage();
	void PrevPage();
	char* Wrap(const char* text, int Width, int fontsize);
	void SplitText(const char* wrapped, int fontsize);
	void setText(const char* newText);
	void setImage(const char* newImagePath);
	void setBoxImage(const char* newBoxPath);
	int getBoxHeight();
};
