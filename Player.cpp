#include "Player.h"

// Constructor: Initialize a player with a name and color index, setting default money and position.
Player::Player(const string name, const int colorIndx)
{
	setPlayerName(name);
	setMoney(START_SUM);
	setPosition(START_POS);
	isJail = false;
	color = colorIndx; // Assigning color indx
	
}

// Destructor: Clean up player resources, remove assets if any, and clear the player name.
Player::~Player()
{
	
	cout << "kill player " << this->playerName << endl;
	if (!houses.empty())
	{
		removeAssets();
		houses.clear();
	}
	else 
		cout << "player had no assets!\n";
	playerName.clear();
}

// Copy Constructor: Initialize a player with detailed parameters including homes, money, position, jail status, and color.
Player::Player(const string name, vector <Asset*>homes, int money, int pos, bool j, int col)
{
	this->playerName = name;
	this->houses = homes;
	this->myMoney = money;
	this->PlayerPosition = pos;
	this->isJail = j;
	this->color = col;
}

// Overloading assignment (=) operator. Assign values from another player object to this one.
Player* Player::operator=(Player *p)
{
	this->houses = p->houses;
	this->isJail = p->isJail;
	this->myMoney = p->myMoney;
	this->playerName = p->playerName;
	this->PlayerPosition = p->PlayerPosition;
	this->color = p->color;
	return this;
}

// Method to set a player's name
void Player::setPlayerName(string name)
{
	try
	{
		if (name == "") BAD_INPUT;
		playerName = name;
	}
	catch (const char* err)
	{cout << err;}
}

// Payment: Handle the payment transaction between players, including mortgaging assets if necessary.
const bool Player::payment(int toPay, Player* hOwner)
{
	vector <Asset*>::iterator iter = houses.begin();
	if (toPay > 0) // Player gets money
	{
		setMoney(toPay); // Add money to player
		if (!this->houses.empty())
		{
			for (iter; iter != houses.end(); iter++)
			{
				// If player has mortgaged assets, release them
				int tempPrice = (*iter)->getMorgagePrice();
				if ((*iter)->isMortg() && myMoney > tempPrice)
					releaseMortgage(*iter, -tempPrice);
			}
		}
	}
	
	else // Player pays money
	{
		// If payment causes negative balance, mortgage assets
		if (myMoney + toPay < 0) 
		{
			if (!this->houses.empty()) // If player has assets
			{
				cout << "You don't have enough money, mortgage process started:\n";
				while (myMoney + toPay < 0)
				{
					if (!(*iter)->isMortg()) // If asset isn't mortgage
					{
						myMoney += (*iter)->getHousePrice(); // Mortgage it
						(*iter)->setMorgage();
						(*iter)->printSlot();
					}
					iter++;
					// If player mortgaged all assets and still can't afford to pay they lost
					if (iter == houses.end() && myMoney + toPay < 0)
						return playerLost(hOwner);
				}
			}
			else // There are no assets to mortgage
				return playerLost(hOwner);
		}
		// Player can afford to pay
		setMoney(toPay); // 'Take' money from player
		if (hOwner)
		{
			cout << hOwner->getPlayerName() << " had " << hOwner->getMoney() <<" NIS\n";
			hOwner->setMoney(-toPay); // House owner gets money 
			cout << "they now have " << hOwner->getMoney() << " NIS\n\n";
		}
	}
	return true;
}

// Release Mortgage: Pay the mortgage fee and release the mortgage on a house.
void Player::releaseMortgage(Asset *h, const int mortPrice)
{
	// Pay mortgage fee and release mortgage
	setMoney(mortPrice);
	h->freeMortgage();
	cout << playerName << " released mortgage of ";
	h->printSlot();
	cout << "they paid " << mortPrice << " NIS\n";
}

// Buy Asset: Subtract the asset price from the player's money and add the asset to the player's properties.
void Player::buyAsset(Asset *a)
{
	// Substract money from player, add to houses vector
	setMoney(-a->getHousePrice()); // Already checked if sufficient money
	houses.push_back(a);
	// Make asset point to new owner
	a->setOwner(this); 
}

// Remove Assets: Clear all asset pointers from the player and release mortgages.
void Player::removeAssets()
{
	// Player lost/quit, remove asset pointers to him and release mortgage
	cout << this->getPlayerName() << " lost the following assets:" << endl;
	vector <Asset*>::iterator iter = houses.begin();
	for (iter; iter != houses.end(); iter++)
	{
		(*iter)->freeMortgage();
		(*iter)->setOwner();
		(*iter)->printSlot();
	}
}

// Find Mortgage: Increase the mortgage years for all mortgaged assets.
void Player::findMortgage()
{
	// Increase mortgage years to assets that are mortgaged
	vector <Asset*>::iterator iter = houses.begin();
	for (iter; iter != houses.end(); iter++)
	{
		if ((*iter)->isMortg())
			(*iter)->increaseMortgageYears();
	}
}

// Get Houses: Print all houses owned by the player.
void Player::getHouses()
{
	if (!houses.empty())
	{
		vector <Asset*>::iterator iter = houses.begin();
		for (iter; iter != houses.end(); iter++)
			(*iter)->printSlot();
	}
	else
		cout << "nothing!\n==========================\n";
}

// Player Lost: Handle the player's loss, transferring their money to another player if applicable.
bool Player::playerLost(Player* hOwner)
{
	YOU_LOSE;
	// If player pays to someone - give them all his money
	if (hOwner)
		hOwner->setMoney(this->myMoney);
	else myMoney = 0;
	return false;
}
