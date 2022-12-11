#include "Sprite.h"

Sprite::Sprite(const char* filename, SDL_Renderer* renderer, int xpos, int ypos, int width, int height, int rows, int cols, int offsetX, int offsetY)
{
	this->renderer = renderer;

	frameInfo.reserve(rows * cols);

	this->xpos = xpos;
	this->ypos = ypos;
	this->width = width;
	this->height = height;
	this->offsetX = offsetX;
	this->offsetY = offsetY;

	visible = true;

	currentFrame = 0;
	startTime = SDL_GetTicks();
	endTime = 0;
	deltaTime = 0;

	animationState = Sprite::UNKNOWN_ANIMATION;

	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			SDL_Rect temp;

			temp.x = (j * width) + offsetX;
			temp.y = (i * height) + offsetY;
			temp.w = width;
			temp.h = height;

			frameInfo.push_back(temp);
		}
	}

	event.type = SDL_RegisterEvents(1);

	load(filename);
}

Sprite::~Sprite()
{
	SDL_DestroyTexture(image);
}

const string Sprite::UNKNOWN_ANIMATION = "unknown";

void Sprite::load(const char* filename)
{
	image = IMG_LoadTexture(renderer, filename);

	if (!image)
	{
		cerr << "IMG_LoadTexture Error: " << IMG_GetError() << endl;

		exit(1);
	}
}

void Sprite::draw()
{
	if (this->visible)
	{
		SDL_Rect dest = { xpos, ypos, width, height };

		SDL_RenderCopy(renderer, image, &frameInfo[currentFrame], &dest);
	}
}

void Sprite::drawFlip()
{
	if (this->visible)
	{
		SDL_Rect dest = { xpos, ypos, width, height };

		SDL_RenderCopyEx(renderer, image, &frameInfo[currentFrame], &dest, 0, nullptr, SDL_FLIP_HORIZONTAL);
	}
}

void Sprite::updateTime()
{
	endTime = SDL_GetTicks();
}

void Sprite::animate(int startFrame, int endFrame, int fps, bool flipFlag)
{
	if (startFrame < 0 || endFrame >= frameInfo.size())
	{
		cerr << "Animation out of range" << endl;
		exit(1);
	}

	if (currentFrame == startFrame)
	{
		event.user.code = Sprite::ANIMATION_START;
		event.user.data1 = (void*)animationState.c_str();
		event.user.data2 = (void*)currentFrame;

		SDL_PushEvent(&event);
	}

	endTime = SDL_GetTicks();
	deltaTime = endTime - startTime;

	currentFrame = ((((deltaTime * fps) / 1000) - startFrame) % ((endFrame + 1) - startFrame)) + startFrame;

	if (!flipFlag)
		draw();
	else
		drawFlip();

	event.user.code = Sprite::ANIMATION_FRAME_UPDATE;
	event.user.data1 = (void*)animationState.c_str();
	event.user.data2 = (void*)currentFrame;

	SDL_PushEvent(&event);

	if (currentFrame == endFrame)
	{
		event.user.code = Sprite::ANIMATION_END;
		event.user.data1 = (void*)animationState.c_str();
		event.user.data2 = (void*)currentFrame;

		SDL_PushEvent(&event);
	}
}

void Sprite::gotoPos(int x, int y)
{
	xpos = x;
	ypos = y;
}

void Sprite::move(int deltaX, int deltaY)
{
	xpos += deltaX;
	ypos += deltaY;
}

void Sprite::setCurrentFrame(int frame)
{
	if (frame < 0 || frame >= frameInfo.size())
		currentFrame = 0;
	else
		currentFrame = frame;
}

int Sprite::getCurrentFrame()
{
	return currentFrame;
}

SDL_Texture* Sprite::getImage()
{
	return image;
}

int Sprite::getWidth()
{
	return frameInfo[currentFrame].w;
}

int Sprite::getHeight()
{
	return frameInfo[currentFrame].h;
}

int Sprite::getXPos()
{
	return xpos;
}

int Sprite::getYPos()
{
	return ypos;
}

bool Sprite::isVisible()
{
	return visible;
}

void Sprite::setWidth(int width)
{
	frameInfo[currentFrame].w = width;
}

void Sprite::setHeight(int height)
{
	frameInfo[currentFrame].h = height;
}

void Sprite::setXPos(int x)
{
	xpos = x;
}

void Sprite::setYPos(int y)
{
	ypos = y;
}

void Sprite::setVisible(bool visible)
{
	this->visible = visible;
}