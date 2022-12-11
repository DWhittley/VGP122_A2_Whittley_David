#include "Button.h"

Button::Button(const char* filename, SDL_Renderer* renderer, int xpos, int ypos, int width, int height, int row, int cols)
{
	buttonSprite = new Sprite(filename, renderer, xpos, ypos, width, height, row, cols);

	pressed = false;
}

Button::~Button()
{
	delete buttonSprite;
}

void Button::setCurrentButton(int frame)
{
	buttonSprite->setCurrentFrame(frame);
}

void Button::onPress()
{
	pressed = true;
}

void Button::setPressed(bool press)
{
	pressed = press;
}

bool Button::isPressed()
{
	return pressed;
}

void Button::setVisible(bool visible)
{
	buttonSprite->setVisible(visible);
}

bool Button::isVisible()
{
	return buttonSprite->isVisible();
}

int Button::getXPos()
{
	return buttonSprite->getXPos();
}

int Button::getYPos()
{
	return buttonSprite->getYPos();
}

int Button::getWidth()
{
	return buttonSprite->getWidth();
}

int Button::getHeight()
{
	return buttonSprite->getHeight();
}

void Button::draw()
{
	buttonSprite->draw();
}