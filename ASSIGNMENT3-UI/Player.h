#pragma once

#include <vector>
#include <iostream>

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
	bool hasPassed();
	void setPassed();
	bool isDone();
	void setDone();

private:
	vector<Card*> hand;
	bool _hasBust;
	int total;
	bool _hasPassed;
	bool _done;
};