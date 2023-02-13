#pragma once
#include "Asset.h"
#include <algorithm>
#include <vector>

#define START_SUM 150
#define YOU_LOSE cout << "You don't have enough money, you lose!\n"
#define START_POS 1

class Asset;
class Player
{
	string playerName;
	int myMoney = 0;
	int PlayerPosition;
	vector <Asset*> houses;
	bool isJail;

public:
	int color; // player print color

	Player(const string, const int);
	~Player();
	Player(const string, vector <Asset*>, int, int, bool, int); // copy constructor
	Player* operator = (Player*);
	
	const bool getJail() { return isJail; };
	void setJail(bool j) { isJail = j; };
	void setPlayerName(string);
	const string getPlayerName() { return playerName; };
	void setMoney(int const cash) { myMoney += cash; };
	const int getMoney() { return myMoney; };
	void setPosition(int pos) { PlayerPosition = pos; };
	const int getPosition() { return PlayerPosition; };
	const bool payment(int, Player * = nullptr);
	void releaseMortgage(Asset*,const int);
	void buyAsset(Asset*);
	void removeAssets();
	void findMortgage();
	void getHouses();
	bool playerLost(Player*);

};

