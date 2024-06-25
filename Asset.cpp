#include "Asset.h"

// Constructor for Asset class
// Initializes an Asset object with streetName, city, house price, rent fee, owner, and mortgage years
Asset::Asset(string streetName, string city, int hPrice, int fee, Player *p, int years):Slot(streetName)
{
	setCityName(city);
	setHousePrice(hPrice);
	setRent(fee);
	setOwner(p);
}
// Copy constructor for Asset class with additional mortgage flag
// Initializes an Asset object with streetName, city, house price, rent fee, owner, and mortgage years
Asset::Asset(const string Sname, const string Cname, int hPrice, int fee, Player* p, int years, bool mort):Slot(Sname)
{
	this->cityName = Cname;
	this->housePrice = hPrice;
	this->rent = fee;
	this->owner = p;
	this->isMortgage = mort;
}

void Asset::setCityName(const string name)
{
	try {  // Check if the name is empty and throw an error if it is
		if (name == "")  BAD_INPUT;
		this->cityName = name;
	}
	catch (const char* err)
	{
		cout << err;  // Print the error message
	}
}

// Calculates the mortgage price based on house price, years, and interest rate
const int Asset::getMorgagePrice()
{
	// Casting to int because there is no use of cents in real monopoly
	return (int) (this->housePrice + this->housePrice * this->years * INTREST_RATE);
}

// Method to print slot details
// Prints the details of the asset.
const void Asset::printSlot()
{
	//print slot using the Slot method and add specific features
	Slot::printSlot();
	cout << "\nCity Name: " << cityName << endl;
	cout << "Street name: " << this->getSlotName() << endl;
	cout << "Costs " << housePrice << " to buy and " << rent << " to rent"<< endl;
	if (isMortgage)
		cout << "Mortgaged for " << this->years << " years" << endl;
	else
		cout << "Not Mortgaged\n";
	cout << "Owned by: ";
	if (owner)
		cout<< owner->getPlayerName() << endl;
	else
		cout << "nobody" << endl;
	cout << "=================\n" << endl;
}
