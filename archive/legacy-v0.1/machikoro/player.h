//
// Created by 14471 on 2022/9/26.
//

#ifndef PLAYER_HEADER
#define PLAYER_HEADER

#include <cstring>
#include <list>
#include "card.h"

using namespace std;
/* contains everything the player can have */
class errorException
{
private:
    string info;
public:
    errorException(string str){info=str;}
    string getInfo()const {return info;}
};

class Player
{
public:
    list<Card> cards;
    char name[32];
    bool isBot;
    int coins;

    bool amusementPark;
    bool radioTower;
    bool shoppingMall;
    bool trainStation;

    int place;

    int wheatFields;
    int ranches;
    int bakeries;
    int cafes;
    int convenienceStores;
    int forests;
    int stadiums;
    int tvStations;
    int businessCenters;
    int cheeseFactories;
    int furnitureFactories;
    int mines;
    int familyRestaurants;
    int appleOrchards;
    int farmersMarkets;

    ~Player();
    Player(char name[]);
    Player() =default;
    void AddCard(Card card);
    bool roll(int *numRolled, bool *doub);
    void earn(Player *opp1, Player *opp2, int roll);
    void purchase();
    void printItems();
};

#endif
