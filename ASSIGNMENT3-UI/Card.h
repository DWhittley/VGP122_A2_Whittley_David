#pragma once

class Card
{
public:
	Card(char, char, int, bool=false);
	~Card();

	char getFace();
	char getSuit();
	int getCard();

	void setBack(bool);
	bool isBack();

	static const char FACES[14];
	static const char SUITS[5];

private:
	char face;
	char suit;

	int frame;

	bool back;
};