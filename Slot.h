#pragma once
#include <iostream>
#include <string>

using namespace std;

#define BAD_INPUT throw "Bad input! exiting...\n" // Macro for throwing a bad input exception
#define COPY_CTRT throw "Copy Constructor Not Used!\n" // Macro for indicating copy constructor is not used
#define ASSIGN_OPRT throw "Assign Operator Not Used!\n" // Macro for indicating assignment operator is not used
#define DEST throw "Destructor Not Used!\n" // Macro for indicating destructor is not used

class Slot
{
	string slotName;
	 int slotNum;
protected:
	static int slotCounter; // Static counter to keep track of the number of Slot instances

public:
	Slot(const string); // Constructor: Initializes a Slot with a name
	virtual ~Slot() ; // Destructor: Cleans up the Slot
	Slot(const string, int) { cout << "Can't copy Slot\n";}; // Copy Constructor: Not used, prints message
	Slot operator = (string) { cout << "Can't assign Slot\n";}; Assignment (=) Operator: Not used, prints message

	// Getter and setter for slot number
	void setSlotNum(const int); 
	const int getSlotNum() { return slotNum; };
	
	// Getter and setter for slot name
	const string getSlotName() { return slotName; };
	void setSlotName(const string);

 	// Virtual method to print slot details
	virtual const void printSlot();
};

