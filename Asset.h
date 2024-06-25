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
	Asset(string streetName, string city, int, int, Player* p = nullptr, int years = 0); // Constructor to initialize an Asset object
	~Asset() { cout << "Asset Destructor\n";  }; // Destructor
	Asset(const string, const string, int, int, Player*, int, bool); // Copy constructor
	Asset operator = (string) { cout << "can't assign Asset\n";  };  // "Assignment" (=) operator overload
	bool operator <(Asset const &a) { return this->housePrice < a.housePrice; }; // "Less than" (<) operator overload for comparing assets by house price

	virtual const void printSlot();    // Method to print asset details

	// Get/Set city name
	string getCityName() { return cityName; };
	void setCityName(const string);
		
	// Get/Set house/asset price
	const int getHousePrice() { return housePrice; };
	void setHousePrice(int price) { housePrice = price; };
	
	// Get/Set asset rent price
	const int getRent() { return rent; };
	void setRent(int rentPrice) { rent = rentPrice; };
		
	// Get/Set asset owner
	void setOwner(Player* p = nullptr) { owner = p; };
	 Player* getOwner() { return owner; };
	
	// check if asset is mortgaged
	bool isMortg() { return isMortgage; };

	// free mortgaged asset
	void freeMortgage() { isMortgage = false; years = 0; };
		
	// get/increase mortgage years
	const int getMortgageYears() { return years; };
	void increaseMortgageYears() { years++; };
		
	// Get/Set mortgage
	void setMorgage(){ isMortgage = true; years = 1; }
	const int getMorgagePrice();
};

