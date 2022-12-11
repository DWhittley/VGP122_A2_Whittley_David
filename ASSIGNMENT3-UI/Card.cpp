#include "Card.h"

Card::Card(char f, char s, int frm, bool b)
{
	this->face = f;
	this->suit = s;
	this->frame = frm;
	this->back = b;
}

Card::~Card()
{}

char Card::getFace()
{
	return this->face;
}

char Card::getSuit()
{
	return this->suit;
}

int Card::getCard()
{
	return this->frame;
}

void Card::setBack(bool b)
{
	this->back = b;
}

bool Card::isBack()
{
	return this->back;
}

const char Card::FACES[14] = "23456789TJQKA";
const char Card::SUITS[5] = "CDHS";