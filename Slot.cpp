#include "Slot.h"

int Slot::slotCounter = 1; // Initialize static member variable slotCounter to 1

// Constructor: Initialize the Slot with a name and set its slot number
Slot::Slot(const string name)
{
	setSlotName(name);
	setSlotNum(slotCounter);
	slotCounter++;
}

// Destructor: Clean up Slot resources
Slot::~Slot()
{
	slotName.clear(); // Clear the slot name string when the Slot object is destroyed
}

// Set Slot Number: Assign a number to the slot
void Slot::setSlotNum(int num)
{
	slotNum = num;
}

// Set Slot Name: Assign a name to the slot
void Slot::setSlotName(const string name)
{
	try 
	{
		if (name == "") BAD_INPUT; // // If no name is provided, throw an error
		this->slotName = name;
	}
	catch (const char* err)
	{
		cout << err;
	}
}

// Print Slot: Print the slot
const void Slot::printSlot()
{
	cout << "(slot " << getSlotNum() << ") "; // Print slot number in a specific format
}
