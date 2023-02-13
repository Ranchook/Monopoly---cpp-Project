#pragma once
#include "Slot.h"
#include "Player.h"

#define INTREST_RATE 0.1
//official monopoly rules is 10% intrest

class Player;
class Asset :public Slot
{
	string cityName;
	int housePrice;
	int rent;
	Player* owner;
	int years;
	bool isMortgage;

public:
	Asset(string streetName, string city, int, int, Player* p = nullptr, int years = 0);
	~Asset() { cout << "Asset Destructor\n";  };
	Asset(const string, const string, int, int, Player*, int, bool); // copy constructor
	Asset operator = (string) { cout << "can't assign Asset\n";  };
	bool operator <(Asset const &a) { return this->housePrice < a.housePrice; };

	virtual const void printSlot();

	string getCityName() { return cityName; };
	void setCityName(const string);
	const int getHousePrice() { return housePrice; };
	void setHousePrice(int price) { housePrice = price; };
	const int getRent() { return rent; };
	void setRent(int rentPrice) { rent = rentPrice; };
	void setOwner(Player* p = nullptr) { owner = p; };
	 Player* getOwner() { return owner; };
	bool isMortg() { return isMortgage; };
	const int getMortgageYears() { return years; };
	void increaseMortgageYears() { years++; };
	void setMorgage(){ isMortgage = true; years = 1; }
	void freeMortgage() { isMortgage = false; years = 0; };
	const int getMorgagePrice();
};

