// MachiKoro.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

#include "Game.h"

using namespace std;

int main()
{
	srand(static_cast<unsigned int>(time(nullptr)));

	Game* game = Game::getInstance();
	while (!game->is_game_over)
	{
		game->roll_dice();
		if (!game->is_game_over)
		{
			cout << endl << "Appuyez sur Entrée pour continuer...";
			string dummy;
			if (!getline(cin, dummy)) break;
		}
	}

	cout << endl << "Partie terminée. Appuyez sur Entrée pour quitter..." << endl;
	string dummy;
	getline(cin, dummy);
	return 0;
}
