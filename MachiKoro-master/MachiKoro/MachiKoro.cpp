// MachiKoro.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>

#include "Deck.h"
#include "Game.h"

using namespace std;

void view_slots(Game *g)
{
	vector<vector<Card*>> s = g->get_slot();
	for (int i = 0; i < s.size(); i++)
	{
		cout << i << ": " << s[i][0]->get_name() << ": " << s[i].size() << endl;
	}
}


int main()
{
	Game::getInstance()->version_old = Game::getInstance()->choose_game();
	while (!Game::getInstance()->is_game_over)
	{
		Game::getInstance()->roll_dice();
		system("pause");
	}


	char t;
	cin >> t;
    return 0;
}

