#pragma once
#ifndef GAME_CORE_HEADER
#define GAME_CORE_HEADER

#include "player.h"

void maingame(Player* player, Player* opp1, Player* opp2);
void name(Player& player1, Player& player2, Player& player3);
void order(Player& player1, Player& player2, Player& player3);
void win(Player& player1, Player& player2, Player& player3);
void game(Player& player1, Player& player2, Player& player3, int& turn);
void cinbot(Player& player);

#endif