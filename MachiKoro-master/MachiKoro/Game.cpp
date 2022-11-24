#include "stdafx.h"
#include <iomanip>
#include <string>
#include <sstream>
#include "Game.h"
#include <regex>

Game* Game::instance = NULL;
using namespace std;

Game::Game()
{
	this->d = new Deck();
	this->slot_count = 0;
	this->is_game_over = false;
	this->version_old;
	this->turn = 0;
	this->deal();

	this->create_player("Michael",true);
	this->create_player("Dave");
	this->create_player("Bob",true);
	this->create_player("Jim");
}

vector<string> split(string str)//通过空格键分开字符串
{
	stringstream ss(str);
	string item;
	vector<string> tokens;
	while (getline(ss, item, ' ')) {
		tokens.push_back(item);
	}
	return tokens;
}

bool Game::choose_game()
{
	cout << "Welcome to MachiKoro"<<endl<<endl;
	cout << "Here are two version of this game." << endl<<endl;
	cout << "One is ORIGIN version , the other is DLC version" << endl<<endl;
	cout << "Which one do you want to play?(origin/dlc)" << endl;
	string cmds = "(origin)"
		"|(dlc)";
	regex view(cmds);//正则
	while (true)
	{
		string str;
		getline(cin, str);
		vector<string> input = split(str);//通过空格分开字符串，得到input[0]，input[1]
		if (!regex_match(str, view))
		{
			cout << "Unknown Command" << endl;
		}
		else if (input[0] == "origin") return true;
		else if (input[0] == "dlc") return false;
	}
}

void Game::create_player(string name, bool bot)//创造人物，并初始化发送卡片（一个WheatField，一个Bakery）
{
	player* p = new player();
	p->bank = new Bank();
	p->name = name;
	p->isBot = bot;

	YellowCard* c;//黄色卡已经加入到人物卡包里面，但激活状态默认为false
	c = new TrainStation(); p->yellow_cards.push_back(c);
	c = new ShoppingMall(); p->yellow_cards.push_back(c);
	c = new AmusementPark(); p->yellow_cards.push_back(c);
	c = new RadioTower(); p->yellow_cards.push_back(c);

	BlueCard* b;
	b = new WheatField(); p->blue_cards.push_back(b);

	GreenCard* g;
	g = new Bakery(); p->green_cards.push_back(g);

	players.push_back(p);
}

void Game::deal()//从桌面发牌到卡槽，卡槽中有十种卡牌
{
	Card* c;
	bool exists;
	int slotnum = 10;
	if (this->version_old) slotnum = 16;
	while (this->d->deck.size() > 0 && this->slot.size() < slotnum)//桌面上有牌，并且自身卡槽还有剩余空间（小于十种）
	{
		exists = false;
		c = this->d->deck.back();
		for (int i = 0; i < this->slot.size(); i++)
		{
			if (c->get_name().compare(this->slot[i][0]->get_name()) == 0)//判断抽卡是否出现重复种类，若重复添加进slot[i]
			{
				slot[i].push_back(c);
				exists = true;
				break;
			}
		}

		if (!exists)//若不重复添加进slot
		{
			vector<Card*> v;
			v.push_back(c);
			this->slot.push_back(v);
			this->slot_count++;
		}
		this->d->deck.pop_back();
	}
}

Deck* Game::get_deck()//获得桌面
{
	return this->d;
}

vector<vector<Card*>> Game::get_slot()//获得卡槽
{
	return this->slot;
}

void Game::rolling_dice(int dice_count)//骰子置数，一个或者两个
{
	this->dice1 = rand() % 6 + 1;
	this->dice2 = 0;

	if (dice_count == 2)
	{
		this->dice2 = rand() % 6 + 1;
	}
	this->dice = this->dice1 + this->dice2;

}

void print_card(Card* c)//打印卡片，setw限定格式
{

	if (c->get_string_color() == "yellow")//单独设置黄卡的打印
	{
		YellowCard* yc = dynamic_cast<YellowCard*>(c);
		string ifactive;
		if (yc->active == true)
		{
			ifactive = "owned";
		}
		else
		{
			ifactive = "no";
		}
		cout << left <<
			setw(10) << c->get_string_color() <<
			setw(25) << c->get_name() <<
			setw(5) << c->get_cost() <<
			setw(5) << c->get_value() <<
			setw(5) << c->get_low_roll() <<
			setw(5) << c->get_high_roll() << setw(5) << ifactive << endl;
	}
	else
	{
		cout << left <<
			setw(10) << c->get_string_color() <<
			setw(25) << c->get_name() <<
			setw(5) << c->get_cost() <<
			setw(5) << c->get_value() <<
			setw(5) << c->get_low_roll() <<
			setw(5) << c->get_high_roll() << endl;
	}
}

void print_card_heading()
{
	cout << left <<
		setw(10) << "color" <<
		setw(25) << "name" <<
		setw(5) << "cost" <<
		setw(5) << "val" <<
		setw(5) << "low" <<
		setw(5) << "high" << endl;
}

void Game::view_slot_cards(bool cls)//打印卡槽卡片
{
	if (cls) system("cls");//清屏
	for (int i = 0; i < 25; i++)cout << "-";
	cout << "CARTES SUR TABLE";
	for (int i = 0; i < 25; i++)cout << "-";
	cout << "\n\n";
	cout << left <<
		setw(5) << "slot" <<
		setw(5) << "qty";
	print_card_heading();
	for (int i = 0; i < this->slot.size(); i++)
	{
		cout << left <<
			setw(5) << i <<
			setw(5) << this->slot[i].size();
		print_card(this->slot[i][0]);
	}
	cout << endl;
}

void Game::view_player_cards(int index, bool cls)//打印玩家卡片
{
	if (cls) system("cls");
	for (int i = 0; i < 25; i++)cout << "-";
	cout << "CARTES DE JOUEUR";
	for (int i = 0; i < 25; i++)cout << "-";
	cout << "\n\n";
	print_card_heading();
	for (auto it = this->players[index]->blue_cards.begin(); it != this->players[index]->blue_cards.end(); it++) 
		print_card(*it);
	cout << endl;
	for (auto it = this->players[index]->green_cards.begin(); it != this->players[index]->green_cards.end(); it++) 
		print_card(*it); 
	cout << endl;
	for (auto it = this->players[index]->red_cards.begin(); it != this->players[index]->red_cards.end(); it++) 
		print_card(*it);
	cout << endl;
	for (auto it = this->players[index]->purple_cards.begin(); it != this->players[index]->purple_cards.end(); it++)  
		print_card(*it); 
	cout << endl;
	for (auto it = this->players[index]->yellow_cards.begin(); it != this->players[index]->yellow_cards.end(); it++) 
		print_card(*it);
	cout << endl;
}

int Game::player_input(string message)//玩家输入想要的操作
{
	string cmds = "(no)"
		"|(view [0-9]+)"
		"|(view table)"
		"|(acheter [0-9]*)"
		"|(acheter TrainStation)"
		"|(acheter ShoppingMall)"
		"|(acheter AmusementPark)"
		"|(acheter RadioTower)";
	bool isRound1 = true;//用于计算AI购买次数
	int slotnum = 9;
	if (this->version_old)	slotnum = 14;
	regex view(cmds);//正则
	bool complete = false;
	while (true)
	{
		string str;
		if (this->players[this->turn]->isBot)
		{
			if (isRound1)
			{
				str = "acheter ";
				srand(time(0));
				str += to_string(rand() % slotnum);
				cout << str<<endl;
			}
			else str = "no";
		}
		else getline(cin, str);
		isRound1 = false;
		vector<string> input = split(str);//通过空格分开字符串，得到input[0]，input[1]
		if (!regex_match(str, view))
		{
			cout << "Commande non valide" << endl;
		}
		else if (input[0] == "no") return -1;
		else if (input[0] == "view")
		{
			if (input[1] == "table") view_slot_cards(true);

			else if (stoi(input[1]) > this->players.size() - 1)//stoi将string转为int
			{
				cout << "There is no player " << stoi(input[1]) << endl;
				continue;
			}
			else view_player_cards(stoi(input[1]), true);
			cout << message << endl;
		}
		else if (input[0] == "acheter" && input[1] == "TrainStation")
		{
			if (this->players[this->turn]->yellow_cards[0]->active)
			{
				cout << "Vous l'avez deja" << endl;
				continue;
			}
			if (this->players[this->turn]->bank->get_coins() >= this->players[this->turn]->yellow_cards[0]->get_cost())
			{
				this->players[this->turn]->yellow_cards[0]->active = true;//TODO:cost money(qyb)
				this->players[this->turn]->bank->withdraw(this->players[this->turn]->yellow_cards[0]->get_cost());
				return (this->players[this->turn]->bank->get_coins());
			}
			else
			{
				cout << "You can't afford that" << endl;
				continue;
			}
		}
		else if (input[0] == "acheter" && input[1] == "ShoppingMall")
		{
			if (this->players[this->turn]->yellow_cards[1]->active)
			{
				cout << "Vous l'avez deja" << endl;
				continue;
			}
			if (this->players[this->turn]->yellow_cards[0]->active && this->players[this->turn]->bank->get_coins() >= this->players[this->turn]->yellow_cards[1]->get_cost())
			{
				this->players[this->turn]->yellow_cards[1]->active = true;//TODO:cost money(qyb)
				this->players[this->turn]->bank->withdraw(this->players[this->turn]->yellow_cards[1]->get_cost());

				return (this->players[this->turn]->bank->get_coins());
			}
			else
			{
				cout << "You cant afford that" << endl;
				continue;
			}
		}
		else if (input[0] == "acheter" && input[1] == "AmusementPark")
		{
			if (this->players[this->turn]->yellow_cards[2]->active)
			{
				cout << "Vous l'avez deja" << endl;
				continue;
			}
			if (this->players[this->turn]->yellow_cards[0]->active && this->players[this->turn]->yellow_cards[1]->active && this->players[this->turn]->bank->get_coins() >= this->players[this->turn]->yellow_cards[2]->get_cost())
			{
				this->players[this->turn]->yellow_cards[2]->active = true;//TODO:cost money(qyb)
				this->players[this->turn]->bank->withdraw(this->players[this->turn]->yellow_cards[2]->get_cost());

				
				return (this->players[this->turn]->bank->get_coins());
			}
			else
			{
				cout << "You can't afford that" << endl;
				continue;
			}
		}
		else if (input[0] == "acheter" && input[1] == "RadioTower")
		{
			if (this->players[this->turn]->yellow_cards[3]->active)
			{
				cout << "Vous l'avez deja" << endl;
				continue;
			}
			if (this->players[this->turn]->yellow_cards[0]->active && this->players[this->turn]->yellow_cards[1]->active && this->players[this->turn]->yellow_cards[2]->active && this->players[this->turn]->bank->get_coins() >= this->players[this->turn]->yellow_cards[3]->get_cost())
			{
				this->players[this->turn]->yellow_cards[3]->active = true;//TODO:cost money(qyb)
				this->players[this->turn]->bank->withdraw(this->players[this->turn]->yellow_cards[3]->get_cost());

				return (this->players[this->turn]->bank->get_coins());
			}
			else
			{
				cout << "You can't afford that" << endl;
				continue;
			}
		}
		else if (input[0] == "acheter")
		{
			if (stoi(input[1]) > this->slot.size())
			{
				cout << "No card at number: " << stoi(input[1]) << endl;
				continue;
			}
			if (this->players[this->turn]->bank->get_coins() < this->slot[stoi(input[1])][0]->get_cost())
			{
				cout << "You can't afford that" << endl;
				continue;
			}
			if (stoi(input[1]) < this->slot.size() && this->players[this->turn]->bank->get_coins() >= this->slot[stoi(input[1])][0]->get_cost())
			{
				this->players[this->turn]->bank->withdraw(this->slot[stoi(input[1])][0]->get_cost());
			}
			return stoi(input[1]);
		}
	}
	return 'a';
}

void Game::roll_dice()//掷骰子主程序
{
	system("cls");//清屏
	this->deal();//判断卡槽是否需要加牌（并加牌）
	this->view_slot_cards(true);//打印卡槽卡片
	this->view_player_cards(this->turn, false);//打印当前玩家卡片

	int dice_count = 1;//默认骰子为1个

	// Should be Train Station Card
	if (this->players[this->turn]->yellow_cards[0]->active)
	{
		string d;
		bool s=true;
		cout << "1 or 2 dice: ";
		string cmds = "(1)"
			"|(2)";
		regex view(cmds);//正则
		while (s)
		{
			string str;
			getline(cin, str);
			vector<string> input = split(str);//通过空格分开字符串，得到input[0]，input[1]
			if (!regex_match(str, view))
			{
				cout << "Unknown Command" << endl;
			}
			else if (input[0] == "1") { dice_count = atoi(str.c_str()); s= false; }
			else if (input[0] == "2") { dice_count = atoi(str.c_str()); s = false; }
			else
				s= true;
		}
	}
	this->rolling_dice(dice_count);

	// Should be Radio Tower（如果有Radio Tower，可以重新掷骰子）
	if (this->players[this->turn]->yellow_cards[3]->active)
	{
		cout << "Rolled a " << this->dice << ". Reroll(y/n): ";
		char response;
		cin >> response;
		if (response == 'y') this->rolling_dice(dice_count);
	}

	cout << endl;
	for (int i = 0; i < 25; i++)cout << "-";
	cout << "STATUT DU JOUEUR";
	for (int i = 0; i < 25; i++)cout << "-";
	cout << "\n\n";
	cout << "Joueur: " << this->turn << endl <<
		"Roulé un: " << this->dice << endl << 
		"Pièces de pré-règlement: " << this->players[this->turn]->bank->get_coins() << endl;
	this->red_card_check();//先进行红卡的判定
}

void Game::red_card_check()//判断是否存在红色卡片，
{
	int tracker = this->turn - 1;
	if (tracker < 0) tracker = this->players.size() - 1;

	while (tracker != this->turn)//对除自身之外其他玩家使用
	{
		for (int i = 0; i < this->players[tracker]->red_cards.size(); i++)
		{
			if (this->dice <= this->players[tracker]->red_cards[i]->get_high_roll() &&
				this->dice >= this->players[tracker]->red_cards[i]->get_low_roll())
			{
				Card* c = NULL;
				// Should be Shopping Mall
				if (this->players[tracker]->yellow_cards[2]->active) c = this->players[tracker]->red_cards[i];
				this->players[tracker]->red_cards[i]->action(//action在继承类中重新定义，执行红卡扣钱操作
					this->players[tracker]->bank,
					this->players[this->turn]->bank,
					c,
					NULL,
					0
				);
			}
			//if (this->players[i]->bank->get_coins() == 0) break;
		}
		tracker--;
		if (tracker < 0) tracker = this->players.size() - 1;

	}
	this->blue_card_check();//进行蓝卡的判定
}
void Game::blue_card_check()//对所有有此蓝卡玩家有效
{
	for (int i = 0; i < this->players.size(); i++)
	{
		for (int j = 0; j < this->players[i]->blue_cards.size(); j++)
		{
			if (this->dice <= this->players[i]->blue_cards[j]->get_high_roll() &&
				this->dice >= this->players[i]->blue_cards[j]->get_low_roll())
			{
				this->players[i]->blue_cards[j]->action(//action在继承类中重新定义
					this->players[i]->bank,
					NULL,
					NULL,
					NULL,
					0
				);
			}
		}
	}
	this->green_card_check();//进行绿卡的判定
}
void Game::green_card_check()//对自身有效
{
	for (int i = 0; i < this->players[this->turn]->green_cards.size(); i++)
	{
		if (this->dice <= this->players[this->turn]->green_cards[i]->get_high_roll() &&
			this->dice >= this->players[this->turn]->green_cards[i]->get_low_roll())
		{
			Card* c = NULL;
			// Should be Shopping Mall
			if (this->players[this->turn]->yellow_cards[2]->active) c = this->players[this->turn]->red_cards[i];
			int val = 0;
			Icon icon = this->players[this->turn]->green_cards[i]->get_icon();
			if (icon != Icon::none)
			{
				for (int j = 0; j < this->players[this->turn]->blue_cards.size(); j++)
				{
					if (icon == this->players[this->turn]->blue_cards[j]->get_icon()) val++;
				}
			}
			this->players[this->turn]->green_cards[i]->action(//action在继承类中重新定义
				this->players[this->turn]->bank,
				NULL,
				c,
				NULL,
				val
			);
		}
	}
	this->purple_card_check();//进行紫卡的判定
}

// TODO: Refactor this. Especially the card swapping. It u-g-l-y
void Game::purple_card_check()
{
	for (int i = 0; i < this->players[this->turn]->purple_cards.size(); i++)
	{
		if (this->players[this->turn]->purple_cards[i]->get_name().compare("Stadium") == 0)
		{
			for (int j = 0; j < this->players.size(); j++)
			{
				this->players[this->turn]->bank->deposit(this->players[j]->bank->withdraw(2));
			}
		}
		if (this->players[this->turn]->purple_cards[i]->get_name().compare("TV Station") == 0)
		{
			cout << "Take up to 5 coins from: ";
			for (int j = 0; j < this->players.size(); j++)
			{
				cout << j << ": " << this->players[j]->bank->get_coins() << " ";
			}
			int player_num;
			cin >> player_num;
			this->players[this->turn]->bank->deposit(this->players[player_num]->bank->withdraw(5));
		}
		if (this->players[this->turn]->purple_cards[i]->get_name().compare("Business Center") == 0)
		{
			cout << "Trade Cards with a player:" << endl;
			for (int j = 0; j < this->players.size(); j++)
			{
				cout << "Player: " << j << endl;
				cout << "Blue(1): ";
				for (int k = 0; i < this->players[j]->blue_cards.size(); k++)
				{
					cout << " |" << k << ": " << this->players[j]->blue_cards[k]->get_name();
				}
				cout << endl;
				cout << "Green(2): ";
				for (int k = 0; i < this->players[j]->green_cards.size(); k++)
				{
					cout << " |" << k << ": " << this->players[j]->green_cards[k]->get_name();
				}
				cout << endl;
				cout << "Red(3): ";
				for (int k = 0; i < this->players[j]->red_cards.size(); k++)
				{
					cout << " |" << k << ": " << this->players[j]->red_cards[k]->get_name();
				}
				cout << endl;
			}
			int player_num;
			int color_num;
			int card_num;
			cout << "Select Player to take: ";
			cin >> player_num;
			cout << endl;

			cout << "Select Color to take: ";
			cin >> color_num;
			cout << endl;

			cout << "Select Card to take: ";
			cin >> card_num;
			cout << endl;

			if (color_num == 1)
			{
				this->players[this->turn]->blue_cards.push_back(this->players[player_num]->blue_cards[card_num]);
				this->players[player_num]->blue_cards.erase(this->players[player_num]->blue_cards.begin() + card_num);
			}
			else if (color_num == 2)
			{
				this->players[this->turn]->green_cards.push_back(this->players[player_num]->green_cards[card_num]);
				this->players[player_num]->green_cards.erase(this->players[player_num]->green_cards.begin() + card_num);
			}
			else
			{
				this->players[this->turn]->red_cards.push_back(this->players[player_num]->red_cards[card_num]);
				this->players[player_num]->red_cards.erase(this->players[player_num]->red_cards.begin() + card_num);
			}

			cout << "Select Color to give: ";
			cin >> color_num;
			cout << endl;

			cout << "Select Card to give: ";
			cin >> card_num;
			cout << endl;

			if (color_num == 1)
			{
				this->players[player_num]->blue_cards.push_back(this->players[this->turn]->blue_cards[card_num]);
				this->players[this->turn]->blue_cards.erase(this->players[this->turn]->blue_cards.begin() + card_num);
			}
			else if (color_num == 2)
			{
				this->players[player_num]->green_cards.push_back(this->players[this->turn]->green_cards[card_num]);
				this->players[this->turn]->green_cards.erase(this->players[this->turn]->green_cards.begin() + card_num);
			}
			else
			{
				this->players[player_num]->red_cards.push_back(this->players[this->turn]->red_cards[card_num]);
				this->players[this->turn]->red_cards.erase(this->players[this->turn]->red_cards.begin() + card_num);
			}
		}
	}
	this->buy_propery();//进入买卡操作
}

// TODO: Allow buying of property
void Game::buy_propery()
{
	cout << "Pièces après règlement: " << this->players[this->turn]->bank->get_coins() << endl;
	for (int i = 0; i < 70; i++)cout << "-";
	cout << "\n\n";
	cout << "Lequel voulez-vous acheter (no or 0-9 or TrainStation/ShoppingMall/AmusementPark/RadioTower)?" << endl;
	int selection;
	bool complete = false;
	while (!complete)
	{
		selection = this->player_input("Lequel voulez-vous acheter (no or 0-9 or TrainStation/ShoppingMall/AmusementPark/RadioTower)?");//选择要买的卡号
		cout << endl;
		if (selection > 0)
		{
			int select = selection;
			if (Color::blue == this->slot[select][0]->get_color())//根据颜色对卡片进行分类加入玩家手中
			{
				this->players[this->turn]->blue_cards.push_back((BlueCard*)this->slot[select].back());//将卡槽中对应卡片加入到玩家手中
				this->slot[select].pop_back();//丢出卡槽中的卡
				complete = true;
			}
			else if (Color::green == this->slot[select][0]->get_color())
			{
				this->players[this->turn]->green_cards.push_back((GreenCard*)this->slot[select].back());
				this->slot[select].pop_back();
				complete = true;
			}
			else if (Color::red == this->slot[select][0]->get_color())
			{
				this->players[this->turn]->red_cards.push_back((RedCard*)this->slot[select].back());
				this->slot[select].pop_back();
				complete = true;
			}
			else if (Color::purple == this->slot[select][0]->get_color())
			{
				bool purp = true;
				for (int i = 0; i < this->players[this->turn]->purple_cards.size(); i++)
				{
					if (this->slot[select].back()->get_name().compare(this->players[this->turn]->purple_cards[i]->get_name()) == 0)
					{
						cout << "You already have this establishment" << endl;
						purp = false;
					}
				}
				if (purp)
				{
					this->players[this->turn]->purple_cards.push_back((PurpleCard*)this->slot[select].back());
					this->slot[select].pop_back();
					complete = true;
				}
			}
			else if (Color::yellow == this->slot[select][0]->get_color())//黄色卡片的购买还没有写，因为黄色卡片不是从卡槽中抽取，大家开局都有黄卡，但是状态不是active
			{
				//TODO: Implement buying mello yello(因为黄卡已经在玩家手上，所以这块不用做）
			}
			if (this->slot[select].size() == 0)
			{
				this->slot.erase(this->slot.begin() + select);
			}
		}
		else
		{
			break;
		}
	}
	this->end_of_turn();
}

void Game::end_of_turn()//对是否结束进行判定，并且进入下一玩家回合
{
	this->is_game_over = true;
	for (int i = 0; i < this->players[turn]->yellow_cards.size(); i++)
	{
		if (!this->players[turn]->yellow_cards[i]->active)
		{
			this->is_game_over = false;
			break;
		}
	}
	for (int i = 0; i < 25; i++)cout << "-";
	cout << "REGLEMENT ROND";
	for (int i = 0; i < 25; i++)cout << "-";
	cout << "\n\n";
	if (this->is_game_over) { cout << this->turn << " wins" << endl; return; }//判断是否获胜
	cout << "Fin du tour de " << this->turn << endl<< "Pièces après l’achat: " << this->players[turn]->bank->get_coins();//打印本轮玩家所剩硬币
	this->turn++;

	// Should be Amusement Park card
	if (this->players[this->turn - 1]->yellow_cards[2]->active)
	{
		turn--;
	}
	if (this->turn == players.size()) this->turn = 0;
	cout << endl << "Tour de "<<this->turn << " après" << endl;//打印下轮玩家
	return;
}