#pragma once
#ifndef CARD_HEADER
#define CARD_HEADER

extern enum CardColor;
extern enum CardName;
class Card
{
public :
	CardColor color;
	CardName name;
	int cost;
	int dice1, dice2;
	bool parambool;
	Card(CardName name);
	~Card();
};
#endif