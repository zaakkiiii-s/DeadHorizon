#pragma once
#include "raylib.h"

// Header file for a sprite class

class sprite
{
public:
	sprite();
	sprite(const char* imagePath, Vector2 imagePosition, float scale);
	~sprite();
	void Draw();
	float getWidth();
	float getHeight();
	Texture2D getTexture();
	Rectangle getBoundingBox();
	Texture2D reloadTexture(const char* imagePath);
	void setTexture(Texture2D newTexture);

protected:
	float originalHeight, originalWidth;
	Texture2D texture;
	Vector2 position;
	Rectangle BoundingBox;

};

// button class inherited from sprite class. Inherits attributes from sprite class

class button : public sprite
{
public:
	button(const char* imagePath, Vector2 imagePosition, float scale);
	~button();
	bool isPressed(Vector2 mousePos, bool mousePressed);

};
