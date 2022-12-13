#pragma once

#include <vector>

#include "Card.h"

using namespace std;

class Player
{
public:
	Player();
	~Player();

	void addCard(Card*);

	Card* getCard(int);
	Card* popCard();
	int getTotal();
	int getCardCount();
	bool hasBust();
	void setBusted();

private:
	vector<Card*> hand;
	bool _hasBust;
	int total;
};