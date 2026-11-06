#include <iostream>
using namespace std;
//
// Created by 14471 on 2022/9/26.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <list>
#include "game_core.h"
#include "string_functions.h"
#include "card.h"
#pragma warning (disable:4996)

extern char firstPlayer[];
extern char secondPlayer[];
extern char thirdPlayer[];
Player::Player(char name[])
{
    list<Card> cards;
    wheatFields = 1;
    bakeries = 1;
    coins = 3;
    ranches = 0;
    cafes = 0;
    convenienceStores = 0;
    forests = 0;
    stadiums = 0;
    tvStations = 0;
    businessCenters = 0;
    cheeseFactories = 0;
    furnitureFactories = 0;
    mines = 0;
    familyRestaurants = 0;
    appleOrchards = 0;
    farmersMarkets = 0;
    trainStation = false;
    shoppingMall = false;
    amusementPark = false;
    radioTower = false;
    isBot=false;
    strcpy(this->name,name);
}

void Player::AddCard(Card card)
{
    cards.push_back(card);
}
/* prints the inputted player's items */
void Player::printItems()
{
    printf("you have %d coins, %d wheat fields, %d ranches, %d bakeries, %d cafes, %d convenience stores, %d forests, %d stadiums, %d TV stations, %d business centers, %d cheese factories, %d furniture factories, %d mines, %d family restaurants, %d apple orchards, %d farmers markets.\n", coins, wheatFields, ranches, bakeries, cafes, convenienceStores, forests, stadiums, tvStations, businessCenters, cheeseFactories, furnitureFactories, mines, familyRestaurants, appleOrchards, farmersMarkets);

    return;
}

Player::~Player() {
}

/* rolls the dice, either rolling with one die or two dice */
bool Player::roll(int *numRolled, bool *doub)
{
    srand(time(0));

    int roll1, roll2;

    if (trainStation == false)
    {
        *numRolled = rand() % 6 + 1;
        return false;
    }
    else
    {
        srand(time(0));
        roll1 = rand() % 6 + 1;
        srand(time(0));
        roll2 = rand() % 6 + 1;
        *numRolled = roll1 + roll2;
        if (roll1 = roll2 && amusementPark)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

/* earn income */
void Player::earn(Player *opp1, Player *opp2, int roll)
{
    srand(time(0));

    switch (roll)
    {
        case 1:
            coins += wheatFields;
            opp1->coins += opp1->wheatFields;
            opp2->coins += opp2->wheatFields;
            break;

        case 2:
            coins += ranches;
            if (shoppingMall)
                coins += bakeries + bakeries;
            else
                coins += bakeries;
            opp1->coins += opp1->ranches;
            opp2->coins += opp2->ranches;
            break;

        case 3:
            coins -= opp1->cafes;
            coins -= opp2->cafes;
            if (shoppingMall)
                coins += bakeries + bakeries;
            else
                coins += bakeries;
            if (opp1->shoppingMall)
                opp1->coins += opp1->cafes + opp1->cafes;
            else
                opp1->coins += opp1->cafes;
            if (opp2->shoppingMall)
                opp2->coins += opp2->cafes + opp2->cafes;
            else
                opp2->coins += opp2->cafes;
            break;

        case 4:
            if (shoppingMall)
                coins += convenienceStores * 3 + (convenienceStores);
            else
                coins += convenienceStores * 3;
            break;

        case 5:
            coins += forests;
            opp1->coins += opp1->forests;
            opp2->coins += opp2->forests;
            break;

        case 6:
            coins += stadiums * 4;
            opp1->coins -= stadiums * 2;
            opp2->coins -= stadiums * 2;

            coins += tvStations * 5;
            char playerToLose[32];
            if (tvStations > 0)
            {
                for (int i = 0; i < tvStations; i++)
                {
                    printf("Which player would you like to take %d from from your TV Station?\n", tvStations * 5);
                    cin>>playerToLose;
                    if (playerToLose == opp1->name)
                    {
                        opp1->coins -= tvStations * 5;
                    }
                    else
                    {
                        opp2->coins -= tvStations * 5;
                    }
                }
            }

            if (businessCenters > 0)
            {
                for (int i = 0; i < businessCenters; i++)
                {
                    int establishment;
                    cout<<"Which non skyscraper establishment would you like to exchange for one of your opponent's? 1 for wheat field, 2 for ranch, 3 for bakery, 4 for cafe, 5 for convenience store \n6 for forest, 7 for cheese factory, 8 for furniture factory, 9 for mine, 10 for family restaurant, 11 for apple orchard, 12 for farmers market.\n";
                    cin>> establishment;
                    cout<<"Which player would you like to take it from?\n";
                    cin>>playerToLose;
                    if (playerToLose == opp1->name)
                    {
                        switch (establishment)
                        {
                            case 1:
                                opp1->wheatFields--;
                                wheatFields++;
                                break;

                            case 2:
                                opp1->ranches--;
                                ranches++;
                                break;

                            case 3:
                                opp1->bakeries--;
                                bakeries++;
                                break;

                            case 4:
                                opp1->cafes--;
                                cafes++;
                                break;

                            case 5:
                                opp1->convenienceStores--;
                                convenienceStores++;
                                break;

                            case 6:
                                opp1->forests--;
                                forests++;
                                break;

                            case 7:
                                opp1->cheeseFactories--;
                                cheeseFactories++;
                                break;

                            case 8:
                                opp1->furnitureFactories--;
                                furnitureFactories++;
                                break;

                            case 9:
                                opp1->mines--;
                                mines++;
                                break;

                            case 10:
                                opp1->familyRestaurants--;
                                familyRestaurants++;
                                break;

                            case 11:
                                opp1->appleOrchards--;
                                appleOrchards++;
                                break;

                            case 12:
                                opp1->farmersMarkets--;
                                farmersMarkets++;
                                break;
                        }
                    }
                    else
                    {
                        switch (establishment)
                        {
                            case 1:
                                opp2->wheatFields--;
                                wheatFields++;
                                break;

                            case 2:
                                opp2->ranches--;
                                ranches++;
                                break;

                            case 3:
                                opp2->bakeries--;
                                bakeries++;
                                break;

                            case 4:
                                opp2->cafes--;
                                cafes++;
                                break;

                            case 5:
                                opp2->convenienceStores--;
                                convenienceStores++;
                                break;

                            case 6:
                                opp2->forests--;
                                forests++;
                                break;

                            case 7:
                                opp2->cheeseFactories--;
                                cheeseFactories++;
                                break;

                            case 8:
                                opp2->furnitureFactories--;
                                furnitureFactories++;
                                break;

                            case 9:
                                opp2->mines--;
                                mines++;
                                break;

                            case 10:
                                opp2->familyRestaurants--;
                                familyRestaurants++;
                                break;

                            case 11:
                                opp2->appleOrchards--;
                                appleOrchards++;
                                break;

                            case 12:
                                opp2->farmersMarkets--;
                                farmersMarkets++;
                                break;
                        }
                    }
                }
            }
            break;

        case 7:
            coins += ranches * 3;
            break;

        case 8:
            coins += (forests * 3) + (mines * 3);
            break;

        case 9:
            coins += mines * 5;
            opp1->coins += opp1->mines * 5;
            opp2->coins += opp2->mines * 5;
            if (shoppingMall)
                opp1->coins += opp1->familyRestaurants * 2 + (opp1->familyRestaurants);
            else
                opp1->coins += opp1->familyRestaurants * 2;
            if (shoppingMall)
                opp2->coins += opp2->familyRestaurants * 2 + (opp2->familyRestaurants);
            else
                opp2->coins += opp2->familyRestaurants * 2;
            coins -= opp1->familyRestaurants * 2;
            coins -= opp2->familyRestaurants * 2;
            break;

        case 10:
            coins += appleOrchards * 3;
            opp1->coins += opp1->appleOrchards * 3;
            opp2->coins += opp2->appleOrchards * 3;
            opp1->coins += opp1->familyRestaurants * 2;
            opp2->coins += opp2->familyRestaurants * 2;
            coins -= opp1->familyRestaurants * 2;
            coins -= opp2->familyRestaurants * 2;
            break;

        case 11:
            coins += (wheatFields * 2) + (appleOrchards * 2);
            break;

        case 12:
            coins += (wheatFields * 2) + (appleOrchards * 2);
            break;
    }

    return;
}



/* purchase cards */
void Player::purchase()
{
    int card;
    bool haveBuilt=true;
    if (coins > 0)
    {
        printf("\n\nWhich card would you like to purchase? 0 for nothing, 1 for wheat field, 2 for ranch, 3 for bakery, 4 for cafe, 5 for convenience store, 6 for forest, 7 for stadium, 8 for TV station, 9 for business center, 10 for cheese factory, 11 for furniture factory, 12 for mine, 13 for family restaurant, 14 for apple orchard, 15 for farmers market, 16 for train station, 17 for shopping mall, 18 for amusement park, 19 for radio tower.\n");
        cin >> card;
        if (!(card == 0))
        {
            if (card < 15)
            {
                switch (card)
                {
                    case 1:
                        if (coins >= 1)
                        {
                            wheatFields ++;
                            coins --;
                        }
                        else
                        {
                            printf("You cannot afford wheat fields.\n");
                            haveBuilt = false;
                        }
                        break;

                    case 2:
                        if (coins >=  1)
                        {
                            ranches ++;
                            coins -=  1;
                        }
                        else
                        {
                            printf("You cannot afford ranches.\n");
                            haveBuilt = false;
                        }
                        break;

                    case 3:
                        if (coins >=  1)
                        {
                            bakeries ++;
                            coins -=  1;
                        }
                        else
                        {
                            printf("You cannot afford bakeries.\n");
                            haveBuilt = false;
                        }
                        break;

                    case 4:
                        if (coins >=  2)
                        {
                            cafes ++;
                            coins -=  2;
                        }
                        else
                        {
                            printf("You cannot afford cafes.\n");
                            haveBuilt = false;
                        }
                        break;

                    case 5:
                        if (coins >=  2)
                        {
                            convenienceStores ++;
                            coins -=  2;
                        }
                        else
                        {
                            printf("You cannot afford convenience stores.\n");
                            haveBuilt = false;
                        }
                        break;

                    case 6:
                        if (coins >=  3)
                        {
                            forests ++;
                            coins -=  3;
                        }
                        else
                        {
                            printf("You cannot afford forests.\n");
                            haveBuilt = false;
                        }
                        break;

                    case 7:
                        if (coins >=  6)
                        {
                            stadiums ++;
                            coins -=  6;
                        }
                        else
                        {
                            printf("You cannot afford stadiums.\n");
                            haveBuilt = false;
                        }
                        break;

                    case 8:
                        if (coins >=  7)
                        {
                            tvStations ++;
                            coins -=  7;
                        }
                        else
                        {
                            printf("You cannot afford TV stations.\n");
                            haveBuilt = false;
                        }
                        break;

                    case 9:
                        if (coins >=  8)
                        {
                            businessCenters ++;
                            coins -=  8;
                        }
                        else
                        {
                            printf("You cannot afford business centers.\n");
                            haveBuilt = false;
                        }
                        break;

                    case 10:
                        if (coins >=  5)
                        {
                            cheeseFactories ++;
                            coins -=  5;
                        }
                        else
                        {
                            printf("You cannot afford cheese factories.\n");
                            haveBuilt = false;
                        }
                        break;

                    case 11:
                        if (coins >=  3)
                        {
                            furnitureFactories ++;
                            coins -=  3;
                        }
                        else
                        {
                            printf("You cannot afford furniture factories.\n");
                            haveBuilt = false;
                        }
                        break;

                    case 12:
                        if (coins >=  6)
                        {
                            mines ++;
                            coins -=  6;
                        }
                        else
                        {
                            printf("You cannot afford mines.\n");
                            haveBuilt = false;
                        }
                        break;

                    case 13:
                        if (coins >=  3)
                        {
                            familyRestaurants ++;
                            coins -=  3;
                        }
                        else
                        {
                            printf("You cannot afford family restaurants.\n");
                            haveBuilt = false;
                        }
                        break;

                    case 14:
                        if (coins >=  3)
                        {
                            appleOrchards ++;
                            coins -=  3;
                        }
                        else
                        {
                            printf("You cannot afford apple orchards.\n");
                            haveBuilt = false;
                        }
                        break;

                    case 15:
                        if (coins >=  2)
                        {
                            farmersMarkets ++;
                            coins -=  2;
                        }
                        else
                        {
                            printf("You cannot afford farmers markets.\n");
                            haveBuilt = false;
                        }
                        break;
                }
            }
            else
            {
                switch (card)
                {
                    case 16:
                        if (trainStation)
                        {
                            printf("You already have your train station!\n");
                            haveBuilt = false;
                        }
                        else
                        {
                            if (coins > 4)
                            {
                                trainStation = true;
                            }
                            else
                            {
                                printf("You cannot afford the train station!\n");
                                haveBuilt = false;
                            }
                        }
                        break;

                    case 17:
                        if (shoppingMall)
                        {
                            printf("You already have your shopping mall!\n");
                            haveBuilt = false;
                        }
                        else
                        {
                            if (coins > 10)
                            {
                                shoppingMall = true;
                            }
                            else
                            {
                                printf("You cannot afford the shopping mall!\n");
                                haveBuilt = false;
                            }
                        }
                        break;

                    case 18:
                        if (amusementPark)
                        {
                            printf("You already have your amusement park!\n");
                            haveBuilt = false;
                        }
                        else
                        {
                            if (coins > 16)
                            {
                                amusementPark = true;
                            }
                            else
                            {
                                printf("You cannot afford the amusement park!\n");
                                haveBuilt = false;
                            }
                        }
                        break;

                    case 19:
                        if (radioTower)
                        {
                            printf("You already have your radio tower!\n");
                            haveBuilt = false;
                        }
                        else
                        {
                            if (coins > 22)
                            {
                                radioTower = true;
                            }
                            else
                            {
                                printf("You cannot afford the radio tower!\n");
                                haveBuilt = false;
                            }
                        }
                        break;
                }
            }
            if (haveBuilt == true) {
                printf("After those purchases, ");
                printItems();
            }
            else
            {
                Player::purchase();
            }
        }
    }
    return;
}

