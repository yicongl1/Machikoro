#include <iostream>
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "player.h"
#include "string_functions.h"
#pragma warning (disable:4996)

extern char firstPlayer[];
extern char secondPlayer[];
extern char thirdPlayer[];

void maingame(Player* player, Player* opp1, Player* opp2) {
    char ch;
    bool doub = false;
    printf("\n-------------It's %s's turn-------------\n", player->name);
    printf("%s, when you're ready to roll, press type any key and then hit enter.\n", player->name);
    cin>>ch;
    do
    {
        int numRolled;
        player->roll(&numRolled, &doub);

        printf("You rolled %d\n\n", numRolled);

        if (player->radioTower)
        {
            char in;
            printf("Since you have a radio tower, would you like to re roll? y for yes, any other key for no.\n");
            cin>>in;
            if ((in == 'y') || (in == 'Y'))
            {
                player->roll(&numRolled, &doub);

                printf("You rolled %d\n\n", numRolled);
            }
        }

        if (doub)
            printf("Since both \"dice\" rolled the same number, and you have an amusement park, you will get to have another turn!\n");

        player->earn(opp1, opp2, numRolled);

        printf("After earning, ");
        player->printItems();
        if (player->coins)
        {
            player->purchase();
        }
        else
        {
            cout << "Since you have no coins, you can not buy any card£¡";
        }
    } while (doub);
}

void cinbot(Player& player)
{
    char in;
    printf("Do you want %s to be a bot player? y for yes, any other key for no.\n",player.name);
    cin >> in;
    if ((in == 'y') || (in == 'Y'))
    {
        player.isBot = true;
        printf("OK, %s will be a bot player\n", player.name);
    }

}

void name(Player& player1, Player& player2, Player& player3)
{
    char name[32];
    printf("What is the first player's name?\n");
    cin >> name;
    player1 = Player(name);
    cinbot(player1);
    printf("What is the second player's name?\n");
    cin >> name;
    player2 = Player(name);
    cinbot(player2);
    printf("What is the third player's name?\n");
    cin >> name;
    player3 = Player(name);
    cinbot(player3);
}
void order(Player& player1, Player& player2, Player& player3)
{
    int first = rand() % 3 + 1;
    int second = rand() % 3 + 1;
    while (second == first) // making sure that two players aren't both first
    {
        second = rand() % 3 + 1;
    }
    int third = rand() % 3 + 1;
    while (third == first || third == second)
    {
        third = rand() % 3 + 1;
    }


    /* giving the first, second, third players names as strings so we can call
    them by their names later */
    switch (first)
    {
    case 1:
        strcpy(firstPlayer, player1.name);
        break;

    case 2:
        strcpy(firstPlayer, player2.name);
        break;

    case 3:
        strcpy(firstPlayer, player3.name);
        break;
    }
    switch (second)
    {
    case 1:
        strcpy(secondPlayer, player1.name);
        break;

    case 2:
        strcpy(secondPlayer, player2.name);
        break;

    case 3:
        strcpy(secondPlayer, player3.name);
        break;
    }
    switch (third)
    {
    case 1:
        strcpy(thirdPlayer, player1.name);
        break;

    case 2:
        strcpy(thirdPlayer, player2.name);
        break;

    case 3:
        strcpy(thirdPlayer, player3.name);
        break;
    }

    printf("%s will go first, then %s then %s.\n", firstPlayer, secondPlayer, thirdPlayer);
}
void win(Player& player1, Player& player2, Player& player3)
{
    if (player1.trainStation && player1.shoppingMall && player1.amusementPark && player1.radioTower)
        printf("%s has won! Congratulations!\n", player1.name);
    else if (player2.trainStation && player2.shoppingMall && player2.amusementPark && player2.radioTower)
        printf("%s has won! Congratulations!\n", player2.name);
    else
        printf("%s has won! Congratulations!\n", player3.name);
}

void game(Player& player1, Player& player2, Player& player3, int& turn)
{
    switch (turn)
    {
    case 1:
        if (streq(firstPlayer, player1.name))
            maingame(&player1, &player2, &player3);
        else if (streq(firstPlayer, player2.name))
            maingame(&player2, &player1, &player3);
        else
            maingame(&player3, &player2, &player1);

        turn++;

        break;

    case 2:
        if (streq(secondPlayer, player1.name))
            maingame(&player1, &player2, &player3);
        else if (streq(secondPlayer, player2.name))
            maingame(&player2, &player1, &player3);
        else
            maingame(&player3, &player2, &player1);

        turn++;

        break;

    case 3:
        if (streq(thirdPlayer, player1.name))
            maingame(&player1, &player2, &player3);
        else if (streq(thirdPlayer, player2.name))
            maingame(&player2, &player1, &player3);
        else
            maingame(&player3, &player2, &player1);

        turn = 1;

        break;
    }
}
