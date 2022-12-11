#pragma once

#include "Sprite.h"

class Button
{
public:
	Button(const char*, SDL_Renderer*, int, int, int, int, int, int);
	~Button();

	void setCurrentButton(int);

	void onPress();
	void setPressed(bool);
	bool isPressed();

	void setVisible(bool);
	bool isVisible();

	int getXPos();
	int getYPos();
	int getWidth();
	int getHeight();

	void draw();

private:
	bool pressed;

	Sprite* buttonSprite;
};