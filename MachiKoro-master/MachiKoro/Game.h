#pragma once
#include <vector>
#include <string>
#include "Card.h"
#include "Bank.h"
#include "Deck.h"


using namespace std;

class player {
public:
	// 四大建筑固定在 yellow_cards 中的位置
	enum YellowSlot {
		TRAIN_STATION = 0,
		SHOPPING_MALL = 1,
		AMUSEMENT_PARK = 2,
		RADIO_TOWER = 3
	};

	bool isBot;
	string name;
	Bank* bank;
	vector<BlueCard*> blue_cards;
	vector<GreenCard*> green_cards;
	vector<RedCard*> red_cards;
	vector<PurpleCard*> purple_cards;
	vector<YellowCard*> yellow_cards;

	bool yellow_active(int index) const
	{
		return index >= 0 && index < (int)yellow_cards.size()
			&& yellow_cards[index] != nullptr && yellow_cards[index]->active;
	}
};

class Game {
public:
	// player_input 的返回值约定
	enum PurchaseCode {
		NO_PURCHASE = -1,      // 选择 no，结束购买
		YELLOW_PURCHASE = -2   // 已成功购买一张大型建筑
	};

	Game();
	int turn;
	bool version_old;
	bool is_game_over;
	void deal();
	Deck* get_deck();
	vector<vector<Card*>> get_slot();
	vector<player*> players;
	bool choose_game();
	void num_ai_player();
	void create_player(string name, bool bot = false);
	void roll_dice();
	void red_card_check();
	void blue_card_check();
	void green_card_check();
	void purple_card_check();
	void buy_propery();
	void end_of_turn();
	void view_slot_cards(bool cls);
	void view_player_cards(int index, bool cls);
	class iterator :public vector<vector<Card*>>::iterator {
	public:
		iterator(vector<vector<Card*>>::iterator it = vector<vector<Card*>>::iterator()) : vector<vector<Card*>>::iterator(it) {};
	};
	iterator begin()
	{
		return iterator(slot.begin());
	}
	iterator end()
	{
		return iterator(slot.end());
	}
	static Game* getInstance()
	{
		if (instance == nullptr) {
			instance = new Game();
		}

		return instance;
	}
private:
	int dice;
	int dice1;
	int dice2;
	static Game* instance;
	void rolling_dice(int dice_count);
	int player_input(string message);
	bool buy_yellow(int index);
	bool buy_slot(int index);
	void tv_station_effect(TVStation* card);
	void business_center_effect(BusinessCenter* card);
	vector<vector<Card*>> slot;
	Deck* d;
};
