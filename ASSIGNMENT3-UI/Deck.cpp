#include "Deck.h"

Deck::Deck(SDL_Renderer* r)
{
	srand(time(0));

	cards.reserve(Deck::MAXCARDS);

	this->renderer = r;

	this->cardSprite = new Sprite("images/CARDS.png", this->renderer, 0, 0, 234, 328, 6, 9, 2, 2);

	this->cardSprite->setCurrentFrame(1);
	
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 13; ++j)
		{
			this->cardSprite->setCurrentFrame(this->cardSprite->getCurrentFrame() + 1);

			Card* temp = new Card((char)Card::FACES[j], (char)Card::SUITS[i], this->cardSprite->getCurrentFrame());

			cards.push_back(temp);
		}
	}

	this->shuffle();
}

Deck::~Deck()
{
	for (int i = 0; i < cards.size(); ++i)
	{
		Card* temp = cards.back();

		cards.pop_back();

		delete temp;
	}

	delete cardSprite;
}

void Deck::shuffle()
{
	Card* temp = nullptr;
	int rndIndex;

	for (int i = 0; i < MAXCARDS; ++i)
	{
		rndIndex = rand() % MAXCARDS;

		temp = cards[0];
		cards[0] = cards[rndIndex];
		cards[rndIndex] = temp;
	}
}

void Deck::setXPos(int x)
{
	cardSprite->setXPos(x);
}

void Deck::setYPos(int y)
{
	cardSprite->setYPos(y);
}

Card* Deck::getCard()
{
	if (cards.size() > 0)
	{
		Card* temp = cards.back();

		this->cards.pop_back();

		return temp;
	}

	return nullptr;
}

void Deck::drawCard()
{
	if (cards.size() > 0)
	{
		Card* temp = cards.back();

		cards.pop_back();

		if (temp->isBack())
			cardSprite->setCurrentFrame(1);
		else
			cardSprite->setCurrentFrame(temp->getCard());

		cardSprite->draw();
	}
}

void Deck::drawCard(Card* temp)
{
	if (temp->isBack())
		cardSprite->setCurrentFrame(1);
	else
		cardSprite->setCurrentFrame(temp->getCard());

	cardSprite->draw();
}

const int Deck::MAXCARDS = 52;