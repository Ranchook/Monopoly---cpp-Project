#pragma once
#include <iostream>
#include <string>

using namespace std;
#define BAD_INPUT throw "Bad input! exiting...\n"
#define COPY_CTRT throw "Copy Constructor Not Used!\n"
#define ASSIGN_OPRT throw "Assign Operator Not Used!\n"
#define DEST throw "Destructor Not Used!\n"

class Slot
{
	string slotName;
	 int slotNum;
protected:
	static int slotCounter;

public:
	Slot(const string); // constructor
	virtual ~Slot() ; // destructor
	Slot(const string, int) { cout << "Can't copy Slot\n";}; // copy constructor
	Slot operator = (string) { cout << "Can't assign Slot\n";};
	void setSlotNum(const int);
	const int getSlotNum() { return slotNum; };
	const string getSlotName() { return slotName; };
	void setSlotName(const string);

	virtual const void printSlot();
};

