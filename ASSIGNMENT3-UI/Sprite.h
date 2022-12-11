#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "SDL.h"
#include "SDL_image.h"

using namespace std;

class Sprite
{
public:
	Sprite(const char*, SDL_Renderer*, int = 0, int = 0, int = 0, int = 0, int = 0, int = 0, int = 0, int = 0);

	~Sprite();

	void animate(int, int, int, bool=false);
	void move(int, int);
	void gotoPos(int, int);
	void draw();
	void drawFlip();

	void setCurrentFrame(int);
	int getCurrentFrame();

	SDL_Texture* getImage();
	int getWidth();
	int getHeight();
	int getXPos();
	int getYPos();
	bool isVisible();

	void setWidth(int);
	void setHeight(int);
	void setXPos(int);
	void setYPos(int);
	void setVisible(bool);

	void updateTime();

	static const int ANIMATION_START = 3;
	static const int ANIMATION_END = 4;
	static const int ANIMATION_FRAME_UPDATE = 5;

	static const string UNKNOWN_ANIMATION;

protected:
	void load(const char*);

	SDL_Texture* image;

	vector<SDL_Rect> frameInfo;

	int xpos, ypos;
	int width, height;
	int offsetX, offsetY;

	int startTime, endTime, deltaTime;

	bool visible;
	int currentFrame;

	string animationState;

	SDL_Event event;
	SDL_Renderer* renderer;
};