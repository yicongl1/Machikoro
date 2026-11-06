#include <iostream>
using namespace std;
/* Machi Koro Game by Pandasaurus ported to C. All rights
go to their respective owners */

/* TODO (version 0.2): make it so you can choose how many players are playing, make it so you can
have second and third places after the first person wins */

/* Version 0.1 */

#include "game_core.h"
#include "player.h"
#include "string_functions.h"

char firstPlayer[32];
char secondPlayer[32];
char thirdPlayer[32];

Player player1,player2,player3;

int main()
{

    cout<<"Hello and welcome to Machi Koro by Pandasaurus \nported to C++. \nAll rights to their respective owners.\n\n";
    /* making players */
    name(player1,player2,player3);

    /* Deciding which player goes first */
    order(player1,player2,player3);

    int turn = 1;
    bool gameStatus = true;
    while (gameStatus)
    {
        game(player1,player2,player3,turn);
        if ((player1.trainStation || player2.trainStation || player3.trainStation) && (player1.shoppingMall || player2.shoppingMall || player3.shoppingMall) && (player1.amusementPark || player2.amusementPark || player2.amusementPark) && (player1.radioTower || player2.radioTower || player3.radioTower))
            break;
    }
    win(player1,player2,player3);
    player1.~Player();
    player2.~Player();
    player3.~Player();
    return 0;
}
