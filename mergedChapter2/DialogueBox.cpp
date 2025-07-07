#include "DialogueBox.hpp"
#include <cstring>
#include <vector>
#include <sstream>

using namespace std;



DialogueBox::DialogueBox()		// Default constructor for dialogue box
{
	boxWidth = 1000;
	boxHeight = 300;
	text = "EMPTY";
	texture = LoadTextureFromImage(GenImageColor(0, 0, WHITE));
	boxTexture = LoadTextureFromImage(GenImageColor(0, 0, WHITE));
	currentpage = 0;
	fontsize = 0;

}

DialogueBox::DialogueBox(const char* text, const char* imagePath, int font)		// Constructor for creating a new dialogue box
{																		// Takes in a string for the displayed text
	Image img = LoadImage(imagePath);									// And an image to be displayed in the box on the right		
	Image backImg = LoadImage("Image_Resources/AITextbox.png");
	boxHeight = backImg.height;
	boxWidth = backImg.width;
	fontsize = font;
	this->text = text;
	currentpage = 0;


	boxTexture = LoadTextureFromImage(backImg);
	texture = LoadTextureFromImage(img);
	UnloadImage(img);
	UnloadImage(backImg);

	char* wrapped = Wrap(text, boxWidth - 700, fontsize);
	SplitText(wrapped, fontsize);
}

DialogueBox::DialogueBox(const char* text, const char* imagePath, const char* boxImage, int fontSize)
{
	Image img = LoadImage(imagePath);									// And an image to be displayed in the box on the right		
	Image backImg = LoadImage(boxImage);
	boxHeight = backImg.height;
	boxWidth = backImg.width;
	fontsize = fontSize;
	this->text = text;
	currentpage = 0;

	//Loading Dialogue Box Display
	boxTexture = LoadTextureFromImage(backImg);
	texture = LoadTextureFromImage(img);
	UnloadImage(img);
	UnloadImage(backImg);

	char* wrapped = Wrap(text, boxWidth - 700, fontsize);
	SplitText(wrapped, fontsize);
}

//Deconstructor for unloading textures
DialogueBox::~DialogueBox()
{
	UnloadTexture(texture);
	UnloadTexture(boxTexture);
}

//function to wrap text in the dialogue box
char* DialogueBox::Wrap(const char* text, int Width, int fontsize)	
{
	string word, line, wrapped;
	int space = MeasureText(" ", fontsize);
	int lineWidth = 0;
	int wordWidth = 0;
	int length = strlen(text);
	char c;

	//Loop through each character of the string
	for (int i = 0; i < length; i++)
	{
		c = text[i];

		if ((c == ' ') || (c == '\n'))
		{
			wordWidth = MeasureText(word.data(), fontsize);

			// If exceeding the line width wrap to a new line
			if (lineWidth + wordWidth > Width)
			{
				wrapped += line + '\n';
				line = word;
				lineWidth = wordWidth + space;
			}
			else {
				if (!line.empty()) line += " ";		//Add space if line isn't empty
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
			line += word;	//Add leftover word to current line
		}
	}

	if (!line.empty()) {
		wrapped += line;
	}

	length = wrapped.length();
	char* wrappedtext = new char[length + 1];	//+1 for null terminator
	copy(wrapped.begin(), wrapped.end(), wrappedtext);
	wrappedtext[length] = '\0';
	return wrappedtext;

}

void DialogueBox::NextPage()
{
	int totalPages = textWall.size();
	if (currentpage < totalPages - 1) {
		currentpage++;	 //Move to next page if not at the end
	}
	else
	{
		currentpage = totalPages - 1;	 //Stay on the last page
	}
}

void DialogueBox::PrevPage()
{
	if (currentpage >= 1)
	{
		currentpage--;	//Go to previous page
	}
	else
	{
		currentpage = 0;	//Stay at first page
	}
}

//function to split text into vector elements if the text does not fit on the dialogue box (height wise)
void DialogueBox::SplitText(const char* wrapped, int fontsize) {
	int maxLines = boxHeight / (fontsize + 20);
	int lines = 0;
	string line, currentpage;
	istringstream split(wrapped);

	while (getline(split, line)) {

		if (lines >= maxLines) {
			char* page = new char[currentpage.length() + 1];
			copy(currentpage.begin(), currentpage.end(), page);
			page[currentpage.length()] = '\0';
			textWall.push_back(page);

			currentpage.clear();
			lines = 0;
		}
		currentpage += line + '\n';	 //Append line and newline
		lines++;
	}
	//Add the last page if there’s leftover content
	if (!currentpage.empty()) {
		char* page = new char[currentpage.length() + 1];
		copy(currentpage.begin(), currentpage.end(), page);
		page[currentpage.length()] = '\0';
		textWall.push_back(page);
	}
}

void DialogueBox::DrawDialogueBox()
{
	DrawTexture(boxTexture, 0, 0, WHITE);
	// Add implementation for the image to be displayed in the correct position on the text box
	// Add it after this DrawTexture(), function to make sure it displays over the text box image
	DrawTexture(texture, 38, 40, WHITE);

	DrawText(textWall[currentpage], 280, 50, 20, BLACK);
	int totalPages = textWall.size();


}

void DialogueBox::DrawDialogueBox(int x, int y)
{
	DrawTexture(boxTexture, 0, 0, WHITE);
	// Add implementation for the image to be displayed in the correct position on the text box
	// Add it after this DrawTexture(), function to make sure it displays over the text box image
	DrawTexture(texture, x, y, WHITE);

	DrawText(textWall[currentpage], x * 9 + 50, 50, 20, BLACK);
	int totalPages = textWall.size();


}

//sets the text to be displayed in the dialogue box
void DialogueBox::setText(const char* newText)
{
	text = newText;
	char* wrapped = Wrap(text, boxWidth - 700, fontsize);
	SplitText(wrapped, fontsize);
}

//sets the character image to be displayed in the dialogue box
void DialogueBox::setImage(const char* newImagePath)
{
	Image img = LoadImage(newImagePath);
	texture = LoadTextureFromImage(img);
}

//sets the image of dialogue box
void DialogueBox::setBoxImage(const char* newBoxImage)
{
	Image img = LoadImage(newBoxImage);
	boxTexture = LoadTextureFromImage(img);
}

//returns height 
int DialogueBox::getBoxHeight()
{
	return boxHeight;
}