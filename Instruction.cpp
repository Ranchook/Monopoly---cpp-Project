#include "Instruction.h"

Instruction::Instruction(const string name, slotType t):Slot(name)
{
	setSlotType(t);
}

const slotType Instruction::getSlotType()
{
	return type;
}

void Instruction::setSlotType(const slotType &t)
{
	//STJ = start, ticket, jail
	type = t;
}

const void Instruction::printSlot()
{
	//print slot using the Slot method and add specific features
	Slot::printSlot();
	cout << getSlotName() << endl;
	cout << "=================\n" << endl;
}