#include "Player.h"

Player::Player(const string name, const int colorIndx)
{
	setPlayerName(name);
	setMoney(START_SUM);
	setPosition(START_POS);
	isJail = false;
	color = colorIndx; // assigning color indx
	
}

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

Player::Player(const string name, vector <Asset*>homes, int money, int pos, bool j, int col)
{
	this->playerName = name;
	this->houses = homes;
	this->myMoney = money;
	this->PlayerPosition = pos;
	this->isJail = j;
	this->color = col;
}

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

const bool Player::payment(int toPay, Player* hOwner)
{
	vector <Asset*>::iterator iter = houses.begin();
	if (toPay > 0) // Player gets money
	{
		setMoney(toPay); // add money to player
		if (!this->houses.empty())
		{
			for (iter; iter != houses.end(); iter++)
			{
				//if player has mortgaged assets, release them
				int tempPrice = (*iter)->getMorgagePrice();
				if ((*iter)->isMortg() && myMoney > tempPrice)
					releaseMortgage(*iter, -tempPrice);
			}
		}
	}
	
	else // Player pays money
	{
		// if payment causes negative balance, mortgage assets
		if (myMoney + toPay < 0) 
		{
			if (!this->houses.empty()) // if player has assets
			{
				cout << "You don't have enough money, mortgage process started:\n";
				while (myMoney + toPay < 0)
				{
					if (!(*iter)->isMortg()) // if asset isn't mortgage
					{
						myMoney += (*iter)->getHousePrice(); // mortgage it
						(*iter)->setMorgage();
						(*iter)->printSlot();
					}
					iter++;
					//if player mortgaged all assets and still can't afford to pay s\he lost
					if (iter == houses.end() && myMoney + toPay < 0)
						return playerLost(hOwner);
					
				}
			}
			else // there are no assets to mortgage
				return playerLost(hOwner);
		}
		//player can afford to pay
		setMoney(toPay); // 'take' money from player
		if (hOwner)
		{
			cout << hOwner->getPlayerName() << " had " << hOwner->getMoney() <<" NIS\n";
			hOwner->setMoney(-toPay); //house owner gets money 
			cout << "s/he now have " << hOwner->getMoney() << " NIS\n\n";
		}
	}
	return true;
}

void Player::releaseMortgage(Asset *h, const int mortPrice)
{
	//pay mortgage fee and release mortgage
	setMoney(mortPrice);
	h->freeMortgage();
	cout << playerName << " released mortgage of ";
	h->printSlot();
	cout << "s/he paid " << mortPrice << " NIS\n";
}

void Player::buyAsset(Asset *a)
{
	//substract money from player, add to houses vector
	setMoney(-a->getHousePrice()); // already checked if sufficient money
	houses.push_back(a);
	//make asset point to new owner
	a->setOwner(this); 
}

void Player::removeAssets()
{
	//player lost/quit, remove asset pointers to him and release mortgage
	cout << this->getPlayerName() << " lost the following assets:" << endl;
	vector <Asset*>::iterator iter = houses.begin();
	for (iter; iter != houses.end(); iter++)
	{
		(*iter)->freeMortgage();
		(*iter)->setOwner();
		(*iter)->printSlot();
	}
}

void Player::findMortgage()
{
	//increase mortgage years to assets that are mortgaged
	vector <Asset*>::iterator iter = houses.begin();
	for (iter; iter != houses.end(); iter++)
	{
		if ((*iter)->isMortg())
			(*iter)->increaseMortgageYears();
	}
}

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

bool Player::playerLost(Player* hOwner)
{
	YOU_LOSE;
	//if player pays to someone - give them all his money
	if (hOwner)
		hOwner->setMoney(this->myMoney);
	else myMoney = 0;
	return false;
}