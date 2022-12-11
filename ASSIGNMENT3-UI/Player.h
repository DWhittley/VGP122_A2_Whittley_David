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
	int getTotal();
	int getCardCount();

private:
	vector<Card*> hand;

	int total;
};