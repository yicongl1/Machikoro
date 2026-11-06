#include <iostream>
using namespace std;
//
// Created by 14471 on 2022/9/26.
//
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#pragma warning (disable:4996)

enum CardColor
{
	red = 1,
	blue,
	green,
	purple,
	yellow,
};

enum CardName
{
	wheatFields = 1,
	bakeries,
	coins,
	ranches,
	cafes,
	convenienceStores,
	forests,
	stadiums,
	tvStations,
	businessCenters,
	cheeseFactories,
	furnitureFactories,
	mines,
	familyRestaurants,
	appleOrchards,
	farmersMarkets,
	trainStation,
	shoppingMall,
	amusementPark,
	radioTower,
};

class Card 
{
    CardColor color;
	CardName name;
	int cost;
	int dice1, dice2;
	bool parambool;
	Card(CardName name)
	{
		this->name = name;
	}
	~Card() {};
};