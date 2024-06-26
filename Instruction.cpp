#include "Instruction.h"

// Constructor to initialize an Instruction object
Instruction::Instruction(const string name, slotType t):Slot(name)
{
	setSlotType(t);
}

// Getter method for the slot type
const slotType Instruction::getSlotType()
{
	return type;
}

// Setter method for the slot type
void Instruction::setSlotType(const slotType &t)
{
	//STJ = start, ticket, jail
	type = t;
}

// Method to print the slot details
const void Instruction::printSlot()
{
    	// Call the base class method to print the slot
	Slot::printSlot();
	
	// Print specific details for Instruction slot
	cout << getSlotName() << endl;
	cout << "=================\n" << endl;
}
