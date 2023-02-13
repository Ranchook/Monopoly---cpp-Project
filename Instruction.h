#pragma once
#include "Slot.h"

typedef enum slotType{ Start, Ticket, Jail } slotType;

class Instruction :public Slot
{
	slotType type;

public:
	Instruction(const string, slotType);
	~Instruction() { cout << "Instruction Destructor\n";}; // using Slot destructor
	Instruction(const string a):Slot(a) { cout << "can't copy Instruction\n";}; // copy constructor
	Instruction operator = (string) { cout << "can't assign Instruction\n"; };

	const slotType getSlotType();
	void setSlotType(const slotType&);
	virtual const void printSlot();

};