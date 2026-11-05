#include "stdafx.h"
#include <iomanip>
#include <string>
#include <sstream>
#include <regex>
#include <cstdlib>
#include "Game.h"

Game* Game::instance = NULL;
using namespace std;

namespace {

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

	// 去掉首尾空白，并把连续空白压成一个空格
	string normalize(string str)
	{
		string result;
		bool pending_space = false;
		for (size_t i = 0; i < str.size(); i++)
		{
			char c = str[i];
			if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
			{
				pending_space = true;
				continue;
			}
			if (pending_space && !result.empty()) result += ' ';
			pending_space = false;
			result += c;
		}
		return result;
	}

	bool parse_int(const string& str, int& value)
	{
		try
		{
			size_t pos = 0;
			int parsed = stoi(str, &pos);
			if (pos != str.size()) return false;
			value = parsed;
			return true;
		}
		catch (...)
		{
			return false;
		}
	}

	bool read_int_line(int& value)
	{
		string line;
		if (!getline(cin, line)) return false;
		return parse_int(normalize(line), value);
	}

	// 反复询问一个整数，输入 no 或 EOF 时返回 false
	bool ask_int(const string& prompt, int& value)
	{
		while (true)
		{
			cout << prompt;
			string line;
			if (!getline(cin, line)) return false;
			line = normalize(line);
			if (line == "no") return false;
			if (parse_int(line, value)) return true;
			cout << "Choix invalide" << endl;
		}
	}

	int color_size(player* p, int color)
	{
		if (color == 1) return (int)p->blue_cards.size();
		if (color == 2) return (int)p->green_cards.size();
		if (color == 3) return (int)p->red_cards.size();
		return 0;
	}

	Card* color_card(player* p, int color, int index)
	{
		if (index < 0 || index >= color_size(p, color)) return nullptr;
		if (color == 1) return p->blue_cards[index];
		if (color == 2) return p->green_cards[index];
		if (color == 3) return p->red_cards[index];
		return nullptr;
	}

	Card* remove_color_card(player* p, int color, int index)
	{
		Card* card = color_card(p, color, index);
		if (card == nullptr) return nullptr;
		if (color == 1) p->blue_cards.erase(p->blue_cards.begin() + index);
		else if (color == 2) p->green_cards.erase(p->green_cards.begin() + index);
		else if (color == 3) p->red_cards.erase(p->red_cards.begin() + index);
		return card;
	}

	void add_card(player* p, Card* card)
	{
		switch (card->get_color())
		{
		case Color::blue: p->blue_cards.push_back(static_cast<BlueCard*>(card)); break;
		case Color::green: p->green_cards.push_back(static_cast<GreenCard*>(card)); break;
		case Color::red: p->red_cards.push_back(static_cast<RedCard*>(card)); break;
		case Color::purple: p->purple_cards.push_back(static_cast<PurpleCard*>(card)); break;
		case Color::yellow: break;
		}
	}

	vector<Card*> non_purple_cards(player* p)
	{
		vector<Card*> result;
		for (size_t i = 0; i < p->blue_cards.size(); i++) result.push_back(p->blue_cards[i]);
		for (size_t i = 0; i < p->green_cards.size(); i++) result.push_back(p->green_cards[i]);
		for (size_t i = 0; i < p->red_cards.size(); i++) result.push_back(p->red_cards[i]);
		return result;
	}

	void remove_card(player* p, Card* card)
	{
		if (card == nullptr) return;
		if (card->get_color() == Color::blue)
		{
			for (size_t i = 0; i < p->blue_cards.size(); i++)
				if (p->blue_cards[i] == card) { p->blue_cards.erase(p->blue_cards.begin() + i); return; }
		}
		else if (card->get_color() == Color::green)
		{
			for (size_t i = 0; i < p->green_cards.size(); i++)
				if (p->green_cards[i] == card) { p->green_cards.erase(p->green_cards.begin() + i); return; }
		}
		else if (card->get_color() == Color::red)
		{
			for (size_t i = 0; i < p->red_cards.size(); i++)
				if (p->red_cards[i] == card) { p->red_cards.erase(p->red_cards.begin() + i); return; }
		}
	}

	void print_cards_of_player(player* p, const string& label)
	{
		cout << label << endl;
		cout << "  bleu(1)  : ";
		for (size_t i = 0; i < p->blue_cards.size(); i++)
			cout << " |" << i << ": " << p->blue_cards[i]->get_name();
		cout << endl;
		cout << "  vert(2)  : ";
		for (size_t i = 0; i < p->green_cards.size(); i++)
			cout << " |" << i << ": " << p->green_cards[i]->get_name();
		cout << endl;
		cout << "  rouge(3) : ";
		for (size_t i = 0; i < p->red_cards.size(); i++)
			cout << " |" << i << ": " << p->red_cards[i]->get_name();
		cout << endl;
	}
}

Game::Game()
{
	this->d = new Deck();
	this->is_game_over = false;
	this->version_old = this->choose_game();
	this->turn = 0;
	this->deal();
	this->num_ai_player();
}

void Game::num_ai_player()//创造人物，并初始化发送卡片（一个WheatField，一个Bakery）
{
	system("cls");//清屏
	int num = 0;
	while (true)
	{
		cout << "the number of player:" << endl;
		if (!read_int_line(num))
		{
			if (cin.eof()) { num = 1; break; }
			cout << "invalid number, please rewrite" << endl;
			continue;
		}
		if (num <= 0)
		{
			cout << "invalid number, please rewrite" << endl;
			continue;
		}
		break;
	}

	int ai_num = 0;
	while (true)
	{
		cout << "the number of ai_player (0-" << num << "):" << endl;
		if (!read_int_line(ai_num))
		{
			if (cin.eof()) { ai_num = 0; break; }
			cout << "invalid number, please rewrite" << endl;
			continue;
		}
		if (ai_num < 0 || ai_num > num)
		{
			cout << "invalid number, please rewrite" << endl;
			continue;
		}
		break;
	}

	for (int i = 1; i <= (num - ai_num); i++)
	{
		string name;
		cout << "The " << i << " player's name is " << endl;
		getline(cin, name);
		name = normalize(name);
		if (name.empty()) name = "player" + to_string(i);
		create_player(name);
	}
	for (int ai = 1; ai <= ai_num; ai++)
	{
		create_player("ai" + to_string(ai), true);
	}
}

bool Game::choose_game()
{
	cout << "Bienvenue à MachiKoro" << endl << endl;
	cout << "Voici deux versions de ce jeu" << endl << endl;
	cout << "L'une est la version ORIGINE, l'autre la version DLC" << endl << endl;
	cout << "Lequel voulez-vous jouer ? (origin/dlc)" << endl;
	regex view("(origin)|(dlc)");//正则
	while (true)
	{
		string str;
		if (!getline(cin, str)) return true;
		str = normalize(str);
		if (!regex_match(str, view))
		{
			cout << "Commande inconnue" << endl;
			continue;
		}
		if (str == "origin") return true;
		if (str == "dlc") return false;
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
	while (this->d->deck.size() > 0 && (int)this->slot.size() < slotnum)//桌面上有牌，并且自身卡槽还有剩余空间（小于十种）
	{
		exists = false;
		c = this->d->deck.back();
		for (auto it = this->slot.begin(); it != this->slot.end(); it++)
		{
			if (it->empty()) continue;
			if (c->get_name().compare((*it)[0]->get_name()) == 0)//判断抽卡是否出现重复种类，若重复添加进slot[i]
			{
				it->push_back(c);
				exists = true;
				break;
			}
		}

		if (!exists)//若不重复添加进slot
		{
			vector<Card*> v;
			v.push_back(c);
			this->slot.push_back(v);
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

	if (c->get_string_color() == "jaune")//单独设置黄卡的打印
	{
		YellowCard* yc = dynamic_cast<YellowCard*>(c);
		string ifactive;
		if (yc != nullptr && yc->active == true)
		{
			ifactive = "possédé";
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
		setw(10) << "couleur" <<
		setw(25) << "nom" <<
		setw(5) << "cost" <<
		setw(5) << "val" <<
		setw(5) << "bas" <<
		setw(5) << "haut" << endl;
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
	for (int i = 0; i < (int)this->slot.size(); i++)
	{
		if (this->slot[i].empty()) continue;
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
	if (index < 0 || index >= (int)this->players.size()) return;

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
		"|(voir [0-9]+)"
		"|(voir table)"
		"|(voir tableau)"
		"|(acheter [0-9]+)"
		"|(acheter TrainStation)"
		"|(acheter ShoppingMall)"
		"|(acheter AmusementPark)"
		"|(acheter RadioTower)";
	regex view(cmds);//正则

	while (true)
	{
		string str;
		if (this->players[this->turn]->isBot)
		{
			// AI 只在"买得起、且不是已拥有的紫卡"的选项里随机选择
			player* p = this->players[this->turn];
			vector<string> options;
			for (int i = 0; i < (int)this->slot.size(); i++)
			{
				if (this->slot[i].empty()) continue;
				Card* c = this->slot[i][0];
				if (p->bank->get_coins() < c->get_cost()) continue;
				if (c->get_color() == Color::purple)
				{
					bool owned = false;
					for (int k = 0; k < (int)p->purple_cards.size(); k++)
						if (p->purple_cards[k]->get_name() == c->get_name()) owned = true;
					if (owned) continue;
				}
				options.push_back("acheter " + to_string(i));
			}

			struct Major { int index; const char* name; };
			const Major majors[] = {
				{ player::TRAIN_STATION,  "TrainStation" },
				{ player::SHOPPING_MALL,  "ShoppingMall" },
				{ player::AMUSEMENT_PARK, "AmusementPark" },
				{ player::RADIO_TOWER,    "RadioTower" }
			};
			for (const Major& m : majors)
			{
				YellowCard* yc = p->yellow_cards[m.index];
				if (yc == nullptr || yc->active) continue;
				if (p->bank->get_coins() < yc->get_cost()) continue;
				options.push_back(string("acheter ") + m.name);
			}

			if (options.empty()) str = "no";
			else str = options[rand() % options.size()];
			cout << str << endl;
		}
		else
		{
			if (!getline(cin, str)) return NO_PURCHASE;
			str = normalize(str);
		}

		vector<string> input = split(str);//通过空格分开字符串，得到input[0]，input[1]
		if (input.empty() || !regex_match(str, view))
		{
			cout << "Commande inconnue" << endl;
			cout << message << endl;
			continue;
		}

		if (input[0] == "no") return NO_PURCHASE;

		if (input[0] == "voir")
		{
			if (input.size() < 2)
			{
				cout << "Commande inconnue" << endl;
				continue;
			}
			if (input[1] == "table" || input[1] == "tableau")
			{
				this->view_slot_cards(true);
			}
			else
			{
				int index = 0;
				if (!parse_int(input[1], index) || index < 0 || index >= (int)this->players.size())
				{
					cout << "Il n'y a pas de joueur " << input[1] << endl;
				}
				else
				{
					this->view_player_cards(index, true);
				}
			}
			cout << message << endl;
			continue;
		}

		if (input[0] == "acheter")
		{
			if (input.size() < 2)
			{
				cout << "Commande inconnue" << endl;
				continue;
			}
			const string& what = input[1];
			if (what == "TrainStation")
			{
				if (this->buy_yellow(player::TRAIN_STATION)) return YELLOW_PURCHASE;
				continue;
			}
			if (what == "ShoppingMall")
			{
				if (this->buy_yellow(player::SHOPPING_MALL)) return YELLOW_PURCHASE;
				continue;
			}
			if (what == "AmusementPark")
			{
				if (this->buy_yellow(player::AMUSEMENT_PARK)) return YELLOW_PURCHASE;
				continue;
			}
			if (what == "RadioTower")
			{
				if (this->buy_yellow(player::RADIO_TOWER)) return YELLOW_PURCHASE;
				continue;
			}

			int index = 0;
			if (!parse_int(what, index))
			{
				cout << "Commande inconnue" << endl;
				continue;
			}
			if (index < 0 || index >= (int)this->slot.size() || this->slot[index].empty())
			{
				cout << "Pas de carte au numéro : " << index << endl;
				continue;
			}
			if (this->players[this->turn]->bank->get_coins() < this->slot[index][0]->get_cost())
			{
				cout << "Vous ne pouvez pas vous le permettre" << endl;
				continue;
			}
			return index;
		}

		cout << "Commande inconnue" << endl;
		cout << message << endl;
	}

	return NO_PURCHASE;
}

bool Game::buy_yellow(int index)//购买大型建筑
{
	player* p = this->players[this->turn];
	if (index < 0 || index >= (int)p->yellow_cards.size()) return false;
	YellowCard* card = p->yellow_cards[index];
	if (card == nullptr) return false;

	if (card->active)
	{
		cout << "Vous l'avez deja" << endl;
		return false;
	}
	// 四大建筑可以按任意顺序建造，没有前置条件
	if (p->bank->get_coins() < card->get_cost())
	{
		cout << "Vous ne pouvez pas vous le permettre" << endl;
		return false;
	}

	p->bank->withdraw(card->get_cost());
	card->active = true;
	cout << card->get_name() << " acheté ! Il vous reste "
		<< p->bank->get_coins() << " pièces." << endl;
	return true;
}

bool Game::buy_slot(int index)//购买卡槽中的普通建筑
{
	player* p = this->players[this->turn];
	if (index < 0 || index >= (int)this->slot.size() || this->slot[index].empty())
	{
		cout << "Pas de carte au numéro : " << index << endl;
		return false;
	}

	Card* card = this->slot[index].back();
	// 紫卡不能重复拥有，必须在扣钱之前判断
	if (card->get_color() == Color::purple)
	{
		for (int i = 0; i < (int)p->purple_cards.size(); i++)
		{
			if (p->purple_cards[i]->get_name() == card->get_name())
			{
				cout << "Vous avez déjà cet établissement" << endl;
				return false;
			}
		}
	}
	if (p->bank->get_coins() < card->get_cost())
	{
		cout << "Vous ne pouvez pas vous le permettre" << endl;
		return false;
	}

	p->bank->withdraw(card->get_cost());
	switch (card->get_color())
	{
	case Color::blue:
		p->blue_cards.push_back(static_cast<BlueCard*>(card));
		break;
	case Color::green:
		p->green_cards.push_back(static_cast<GreenCard*>(card));
		break;
	case Color::red:
		p->red_cards.push_back(static_cast<RedCard*>(card));
		break;
	case Color::purple:
		p->purple_cards.push_back(static_cast<PurpleCard*>(card));
		break;
	case Color::yellow:
		break;
	}

	this->slot[index].pop_back();
	if (this->slot[index].empty()) this->slot.erase(this->slot.begin() + index);

	cout << card->get_name() << " acheté ! Il vous reste "
		<< p->bank->get_coins() << " pièces." << endl;
	return true;
}

void Game::roll_dice()//掷骰子主程序
{
	system("cls");//清屏
	this->deal();//判断卡槽是否需要加牌（并加牌）
	this->view_slot_cards(true);//打印卡槽卡片
	this->view_player_cards(this->turn, false);//打印当前玩家卡片

	int dice_count = 1;//默认骰子为1个
	player* current = this->players[this->turn];

	// 拥有火车站时可以选择 1 或 2 个骰子
	if (current->yellow_active(player::TRAIN_STATION))
	{
		if (current->isBot)
		{
			dice_count = 2;//AI 总是选择收益更高的两个骰子
		}
		else
		{
			while (true)
			{
				cout << "1 ou 2 dés : ";
				string str;
				if (!getline(cin, str)) { dice_count = 1; break; }
				str = normalize(str);
				if (str == "1") { dice_count = 1; break; }
				if (str == "2") { dice_count = 2; break; }
				cout << "Commande inconnue" << endl;
			}
		}
	}
	this->rolling_dice(dice_count);

	// 拥有无线电塔时，每回合可以重掷一次
	if (current->yellow_active(player::RADIO_TOWER))
	{
		if (current->isBot)
		{
			if (this->dice <= 4)
			{
				this->rolling_dice(dice_count);
				cout << "Radio Tower : relance -> " << this->dice << endl;
			}
		}
		else
		{
			cout << "Roulé un " << this->dice << ". Relancer (y/n) : ";
			string response;
			if (!getline(cin, response)) response = "n";
			response = normalize(response);
			if (!response.empty() && (response[0] == 'y' || response[0] == 'Y'))
				this->rolling_dice(dice_count);
		}
	}

	cout << endl;
	for (int i = 0; i < 25; i++)cout << "-";
	cout << "STATUT DU JOUEUR";
	for (int i = 0; i < 25; i++)cout << "-";
	cout << "\n\n";
	cout << "Joueur: " << this->turn << endl <<
		"Roulé un: " << this->dice << endl <<
		"Pièces de pré-règlement: " << current->bank->get_coins() << endl;
	this->red_card_check();//先进行红卡的判定
}

void Game::red_card_check()//判断是否存在红色卡片，
{
	int tracker = this->turn - 1;
	if (tracker < 0) tracker = (int)this->players.size() - 1;

	while (tracker != this->turn)//对除自身之外其他玩家使用
	{
		player* owner = this->players[tracker];
		// Shopping Mall 是 yellow_cards[1]，不是 [2]
		Card* mall = owner->yellow_active(player::SHOPPING_MALL)
			? owner->yellow_cards[player::SHOPPING_MALL]
			: nullptr;

		for (int i = 0; i < (int)owner->red_cards.size(); i++)
		{
			RedCard* card = owner->red_cards[i];
			if (this->dice <= card->get_high_roll() &&
				this->dice >= card->get_low_roll())
			{
				card->action(//action在继承类中重新定义，执行红卡扣钱操作
					owner->bank,
					this->players[this->turn]->bank,
					mall,
					nullptr,
					0
				);
			}
		}

		tracker--;
		if (tracker < 0) tracker = (int)this->players.size() - 1;
	}
	this->blue_card_check();//进行蓝卡的判定
}

void Game::blue_card_check()//对所有有此蓝卡玩家有效
{
	for (int i = 0; i < (int)this->players.size(); i++)
	{
		for (int j = 0; j < (int)this->players[i]->blue_cards.size(); j++)
		{
			BlueCard* card = this->players[i]->blue_cards[j];
			if (this->dice <= card->get_high_roll() &&
				this->dice >= card->get_low_roll())
			{
				card->action(//action在继承类中重新定义
					this->players[i]->bank,
					nullptr,
					nullptr,
					nullptr,
					0
				);
			}
		}
	}
	this->green_card_check();//进行绿卡的判定
}

void Game::green_card_check()//对自身有效
{
	player* p = this->players[this->turn];
	Card* mall = p->yellow_active(player::SHOPPING_MALL)
		? p->yellow_cards[player::SHOPPING_MALL]
		: nullptr;

	for (int i = 0; i < (int)p->green_cards.size(); i++)
	{
		GreenCard* card = p->green_cards[i];
		if (this->dice <= card->get_high_roll() &&
			this->dice >= card->get_low_roll())
		{
			int val = 0;
			// 加成数量取决于卡牌的 multiplier 图标（牛/齿轮/谷物），不是卡牌自己的图标
			Icon icon = card->get_multiplier_icon();
			if (icon != Icon::none)
			{
				for (int j = 0; j < (int)p->blue_cards.size(); j++)
				{
					if (icon == p->blue_cards[j]->get_icon()) val++;
				}
			}
			card->action(//action在继承类中重新定义
				p->bank,
				nullptr,
				mall,
				nullptr,
				val
			);
		}
	}
	this->purple_card_check();//进行紫卡的判定
}

void Game::tv_station_effect(TVStation* card)
{
	player* self = this->players[this->turn];
	vector<int> targets;
	for (int j = 0; j < (int)this->players.size(); j++)
		if (j != this->turn) targets.push_back(j);
	if (targets.empty()) return;

	int target = -1;
	if (self->isBot)
	{
		int best = -1;
		for (int j : targets)
		{
			if (this->players[j]->bank->get_coins() > best)
			{
				best = this->players[j]->bank->get_coins();
				target = j;
			}
		}
	}
	else
	{
		cout << "Prenez jusqu'à 5 pièces de : ";
		for (int j : targets)
			cout << j << ": " << this->players[j]->bank->get_coins() << " ";
		cout << endl;
		while (true)
		{
			if (!ask_int("Joueur (no pour annuler) : ", target)) return;
			if (target != this->turn && target >= 0 && target < (int)this->players.size()) break;
			cout << "Choix invalide" << endl;
		}
	}

	if (target < 0) return;
	card->action(self->bank, this->players[target]->bank, nullptr, nullptr, 0);
	cout << "TV Station : " << this->turn << " prend 5 pièces (ou moins) à " << target << endl;
}

void Game::business_center_effect(BusinessCenter* card)
{
	player* self = this->players[this->turn];
	vector<int> targets;
	for (int j = 0; j < (int)this->players.size(); j++)
	{
		if (j == this->turn) continue;
		if (!non_purple_cards(this->players[j]).empty()) targets.push_back(j);
	}
	if (targets.empty() || non_purple_cards(self).empty())
	{
		cout << "Aucun échange possible." << endl;
		return;
	}

	if (self->isBot)
	{
		int target = targets[rand() % targets.size()];
		vector<Card*> mine = non_purple_cards(self);
		vector<Card*> theirs = non_purple_cards(this->players[target]);
		Card* give = mine[rand() % mine.size()];
		Card* take = theirs[rand() % theirs.size()];
		remove_card(self, give);
		remove_card(this->players[target], take);
		add_card(self, take);
		add_card(this->players[target], give);
		cout << "Business Center : " << this->turn << " échange "
			<< give->get_name() << " contre " << take->get_name()
			<< " avec " << target << endl;
		return;
	}

	for (int j : targets)
		print_cards_of_player(this->players[j], "Joueur " + to_string(j) + " :");
	print_cards_of_player(self, "Vos cartes :");
	cout << "(Entrez 'no' pour annuler)" << endl;

	int player_num = -1;
	while (true)
	{
		if (!ask_int("Sélectionnez le joueur à échanger : ", player_num)) return;
		if (player_num != this->turn && player_num >= 0 && player_num < (int)this->players.size()
			&& !non_purple_cards(this->players[player_num]).empty())
			break;
		cout << "Choix invalide" << endl;
	}

	int take_color = 0;
	while (true)
	{
		if (!ask_int("Couleur à prendre (1=bleu, 2=vert, 3=rouge) : ", take_color)) return;
		if (take_color >= 1 && take_color <= 3
			&& color_size(this->players[player_num], take_color) > 0) break;
		cout << "Choix invalide" << endl;
	}

	int take_index = -1;
	while (true)
	{
		if (!ask_int("Carte à prendre : ", take_index)) return;
		if (take_index >= 0 && take_index < color_size(this->players[player_num], take_color)) break;
		cout << "Choix invalide" << endl;
	}

	int give_color = 0;
	while (true)
	{
		if (!ask_int("Couleur à donner (1=bleu, 2=vert, 3=rouge) : ", give_color)) return;
		if (give_color >= 1 && give_color <= 3 && color_size(self, give_color) > 0) break;
		cout << "Choix invalide" << endl;
	}

	int give_index = -1;
	while (true)
	{
		if (!ask_int("Carte à donner : ", give_index)) return;
		if (give_index >= 0 && give_index < color_size(self, give_color)) break;
		cout << "Choix invalide" << endl;
	}

	Card* taken = remove_color_card(this->players[player_num], take_color, take_index);
	Card* given = remove_color_card(self, give_color, give_index);
	add_card(self, taken);
	add_card(this->players[player_num], given);
	cout << "Échange effectué : " << given->get_name()
		<< " contre " << taken->get_name() << endl;
}

void Game::purple_card_check()
{
	// Stadium / TV Station / Business Center 都只在骰出 6 时生效
	if (this->dice == 6)
	{
		player* p = this->players[this->turn];
		for (int i = 0; i < (int)p->purple_cards.size(); i++)
		{
			Card* card = p->purple_cards[i];
			if (Stadium* stadium = dynamic_cast<Stadium*>(card))
			{
				int total = 0;
				for (int j = 0; j < (int)this->players.size(); j++)
				{
					if (j == this->turn) continue;//不向自己收钱
					int before = this->players[j]->bank->get_coins();
					stadium->action(p->bank, this->players[j]->bank, nullptr, nullptr, 0);
					total += before - this->players[j]->bank->get_coins();
				}
				cout << "Stadium : " << this->turn << " reçoit " << total
					<< " pièces des autres joueurs." << endl;
			}
			else if (TVStation* tv = dynamic_cast<TVStation*>(card))
			{
				this->tv_station_effect(tv);
			}
			else if (BusinessCenter* bc = dynamic_cast<BusinessCenter*>(card))
			{
				this->business_center_effect(bc);
			}
		}
	}
	this->buy_propery();//进入买卡操作
}

void Game::buy_propery()
{
	player* current = this->players[this->turn];
	cout << "Pièces après règlement: " << current->bank->get_coins() << endl;
	for (int i = 0; i < 70; i++)cout << "-";
	cout << "\n\n";
	string message = "Lequel voulez-vous acheter (no, un numéro de carte, ou TrainStation/ShoppingMall/AmusementPark/RadioTower)?";
	cout << message << endl;

	while (true)
	{
		int selection = this->player_input(message);//选择要买的卡号
		if (selection == NO_PURCHASE) break;
		if (selection == YELLOW_PURCHASE)
		{
			// 黄卡已在 player_input 中扣钱并激活；一回合只能建造一张
			break;
		}
		if (selection < 0 || selection >= (int)this->slot.size()) continue;

		if (this->buy_slot(selection))
		{
			// 一回合只能建造一张，购买成功后结束购买阶段
			break;
		}
	}
	this->end_of_turn();
}

void Game::end_of_turn()//对是否结束进行判定，并且进入下一玩家回合
{
	this->is_game_over = true;
	player* current = this->players[this->turn];
	for (int i = 0; i < (int)current->yellow_cards.size(); i++)
	{
		if (!current->yellow_cards[i]->active)
		{
			this->is_game_over = false;
			break;
		}
	}
	for (int i = 0; i < 25; i++)cout << "-";
	cout << "REGLEMENT ROND";
	for (int i = 0; i < 25; i++)cout << "-";
	cout << "\n\n";
	if (this->is_game_over)
	{
		cout << current->name << " (joueur " << this->turn << ") wins" << endl;//判断是否获胜
		return;
	}
	cout << "Fin du tour de " << this->turn << endl
		<< "Pièces après l’achat: " << current->bank->get_coins();//打印本轮玩家所剩硬币

	// 游乐园：只有掷出对子才额外获得一回合
	bool extra_turn = current->yellow_active(player::AMUSEMENT_PARK)
		&& this->dice1 == this->dice2;

	this->turn++;
	if (extra_turn)
	{
		this->turn--;
		cout << endl << "Parc d'attractions : le joueur " << this->turn << " rejoue !";
	}
	if (this->turn >= (int)this->players.size()) this->turn = 0;
	cout << endl << "Tour de " << this->turn << " après" << endl;//打印下轮玩家
	return;
}
