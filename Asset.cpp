#include "Asset.h"

Asset::Asset(string streetName, string city, int hPrice, int fee, Player *p, int years):Slot(streetName)
{
	setCityName(city);
	setHousePrice(hPrice);
	setRent(fee);
	setOwner(p);
}

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
	try {
		// if no name throw error
		if (name == "")  BAD_INPUT;
		this->cityName = name;
	}
	catch (const char* err)
	{
		cout << err;
	}
}

//we use int because there is no use of cents\agorot in real monopoly
const int Asset::getMorgagePrice()
{
	return (int) (this->housePrice + this->housePrice * this->years * INTREST_RATE);
}

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
