#pragma once

#include <vector>
#include <ctime>

#include "SDL.h"

#include "Card.h"
#include "Sprite.h"

using namespace std;

class Deck
{
public:
	Deck(SDL_Renderer*);
	~Deck();

	void shuffle();

	void setXPos(int);
	void setYPos(int);

	Card* getCard();

	void drawCard();
	void drawCard(Card*);

	static const int MAXCARDS;

private:
	vector<Card*> cards;

	Sprite* cardSprite;
	SDL_Renderer* renderer;
};
