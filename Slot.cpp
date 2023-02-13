#include "Slot.h"

int Slot::slotCounter = 1;

Slot::Slot(const string name)
{
	setSlotName(name);
	setSlotNum(slotCounter);
	slotCounter++;
}

Slot::~Slot()
{
	slotName.clear();
}

void Slot::setSlotNum(int num)
{
	slotNum = num;
}

void Slot::setSlotName(const string name)
{
	try 
	{
		if (name == "") BAD_INPUT; // if no name throw error
		this->slotName = name;
	}
	catch (const char* err)
	{
		cout << err;
	}
}

const void Slot::printSlot()
{
	//print slot number
	cout << "(slot " << getSlotNum() << ") ";
}
