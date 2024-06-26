#pragma once
#include "Slot.h"

// Define slotType enumeration
typedef enum slotType{ Start, Ticket, Jail } slotType;

class Instruction :public Slot
{
	slotType type;

public:
	Instruction(const string, slotType);     // Constructor to initialize Instruction with name and slot type
	~Instruction() { cout << "Instruction Destructor\n";}; // Destructor for Instruction
	Instruction(const string a):Slot(a) { cout << "can't copy Instruction\n";};     // Copy constructor - not used in this case
	Instruction operator = (string) { cout << "can't assign Instruction\n"; }; // Assignment (=) operator - not used in this case

	const slotType getSlotType();     // Getter for slot type
	void setSlotType(const slotType&);     // Setter for slot type
	virtual const void printSlot();     // Method to print slot details
};
