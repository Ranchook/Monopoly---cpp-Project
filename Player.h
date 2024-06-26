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
	int color; // Player's color index for display

	Player(const string, const int);     // Constructor: Initialize player with name and color index, setting default money and position
	~Player();     // Destructor: Clean up player resources, remove assets if any
	Player(const string, vector <Asset*>, int, int, bool, int);     // Copy Constructor: Initialize player with detailed parameters
	Player* operator = (Player*);     // Assignment Operator: Assign values from another player object

	// Getter and Setter of player jail status
	const bool getJail() { return isJail; };
	void setJail(bool j) { isJail = j; };

	// Getter and Setter of a player's name
	void setPlayerName(string);
	const string getPlayerName() { return playerName; };

	// Getter and Setter of a player's money
	void setMoney(int const cash) { myMoney += cash; };
	const int getMoney() { return myMoney; };

	// Getter and Setter of a player's position on the board
	void setPosition(int pos) { PlayerPosition = pos; };
	const int getPosition() { return PlayerPosition; };

	// Payment method and release mortgage method
	const bool payment(int, Player * = nullptr);
	void releaseMortgage(Asset*,const int);

	// Asset related methods
	void buyAsset(Asset*);
	void removeAssets();
	void findMortgage();
	void getHouses();
	
	bool playerLost(Player*);
};

