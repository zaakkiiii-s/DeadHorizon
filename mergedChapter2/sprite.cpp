#include "raylib.h"
#include "sprite.hpp"

// Default constructor for sprite class
// Ignore the error, it works fine with it

sprite::sprite()
{
	originalHeight = 0;
	originalWidth = 0;
	position = { 0,0 };
	BoundingBox = { 0, 0, 0, 0 };
	texture = LoadTextureFromImage(GenImageColor(0, 0, WHITE));
}

// Constructor for sprite class
// Takes in the image path, image position, and the scale of the image

sprite::sprite(const char* imagePath, Vector2 imagePosition, float scale)
{
	Image image = LoadImage(imagePath);

	originalWidth = float(image.width);
	originalHeight = float(image.height);

	int newWidth = static_cast<int>(originalWidth * scale);
	int newHeight = static_cast<int>(originalHeight * scale);

	ImageResize(&image, newWidth, newHeight);
	texture = LoadTextureFromImage(image);
	BoundingBox = { position.x, position.y, static_cast<float>(texture.width), static_cast<float>(texture.height) };
	UnloadImage(image);

	position = imagePosition;
}

// Destructor for sprite class
// Called automatically when sprite is unloaded

sprite::~sprite()
{
	UnloadTexture(texture);
}

// Draws the sprite at the position provided in the constructor

void sprite::Draw()
{
	DrawTextureV(texture, position, WHITE);
}

// Gets the width of the sprite

float sprite::getWidth()
{
	return originalWidth;
}

// Gets the height of the sprite

float sprite::getHeight()
{
	return originalHeight;
}

// Gets the texture value of the sprite

Texture2D sprite::getTexture()
{
	return texture;
}

Rectangle sprite::getBoundingBox()
{
	return BoundingBox;
}

Texture2D sprite::reloadTexture(const char* imagePath)
{
	Image image = LoadImage(imagePath);
	Texture2D texture = LoadTextureFromImage(image);
	return texture;
}

void sprite::setTexture(Texture2D newTexture)
{
	texture = newTexture;
}

// Contructor for button inherited button class

button::button(const char* imagePath, Vector2 imagePosition, float scale)
{
	Image image = LoadImage(imagePath);

	originalWidth = float(image.width);
	originalHeight = float(image.height);

	int newWidth = static_cast<int>(originalWidth * scale);
	int newHeight = static_cast<int>(originalHeight * scale);

	ImageResize(&image, newWidth, newHeight);
	texture = LoadTextureFromImage(image);
	BoundingBox = { position.x, position.y, static_cast<float>(texture.width), static_cast<float>(texture.height) };
	UnloadImage(image);

	position = imagePosition;
}

// Destructor for inherited button class

button::~button()
{
	UnloadTexture(texture);
}

// Returns true if button is pressed

bool button::isPressed(Vector2 mousePos, bool mousePressed)
{
	Rectangle rect = { position.x, position.y, static_cast<float>(texture.width), static_cast<float>(texture.height) };

	if (CheckCollisionPointRec(mousePos, rect) && mousePressed)
	{
		return true;
	}
	return false;
}