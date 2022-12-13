#include "Player.h"

Player::Player()
{
	total = 0;

	hand.reserve(7);
}

Player::~Player()
{
	for (int i = 0; i < hand.size(); ++i)
	{
		Card* temp = hand.back();

		hand.pop_back();

		delete temp;
	}
}

void Player::addCard(Card* c)
{
	char face = c->getFace();

	hand.push_back(c);

	switch (face)
	{
	case 'T':
	case 'J':
	case 'Q':
	case 'K':
		total += 10;
		break;
	case 'A': // changed this logic from JP's to assess the Ace differently
		total += 1;
		if (total <= 11)
			total += 10;
		break;
	default:
		total += (int)face - 48;
		break;
	}
}

Card* Player::popCard()
{
	Card* temp = hand.back();
	hand.pop_back();

	return temp;
}

Card* Player::getCard(int index)
{
	if (index >= 0 && index <= hand.size() - 1)
		return hand[index];

	return nullptr;
}

int Player::getTotal()
{
	bool hasAce = false;
	int temp = 0; //setting up the temp hand value

	for (int i = 0; i < hand.size(); i++) 
	{
		char face = hand[i]->getFace();
		switch (face)
		{
		case 'T':
		case 'J':
		case 'Q':
		case 'K':
			temp += 10;
			break;
		case 'A': // changed this logic from JP's to assess the Ace differently
			temp += 1;
			hasAce = true;
			break;
		default:
			temp += (int)face - 48;
			break;
		}
	}
	if (hasAce = true && temp < 12)
		temp += 10;
	return temp;

}

int Player::getCardCount()
{
	return hand.size();
}

bool Player::hasBust() {
	return _hasBust;
}

void Player::setBusted() {
	_hasBust = true;
}

bool Player::hasPassed() {
	return _hasPassed;
}

void Player::setPassed() {
	_hasPassed = true;
}

bool Player::isDone() {
	return _done;
}
void Player::setDone() {
	_done = true;
}