#pragma once
#include "Player.h"
#include "Instruction.h"
#include <ctime>
#include <fstream>
#include <list>
#include <queue>
#include <deque>
#include <windows.h>

// Undefine max macro to avoid conflicts with windows.h
#undef max // because of windows.h => used in buffer clean
			// in line 328 @ gameEngine.cpp

#define BOARD "board.txt" // File name for the game board
#define CARDS 20	// Number of cards in the deck
#define ASST dynamic_cast <Asset*>  // Macro for dynamic casting to Asset
#define INST dynamic_cast <Instruction*>  // Macro for dynamic casting to Instruction
#define WHITE_PRINT SetConsoleTextAttribute(hConsole, 15) // Macro to set text color to white
// each player will have their own text color, "Bank" messages are in white.

class GameEngine
{
	deque <Player*> players; // Container for players in the game
	queue <int> deck;	// Queue representing the deck of cards
	list <Slot*> board;	// List of slots on the game board
	int slotTotal = 0;	// Total number of slots on the board
	HANDLE hConsole;	 // Handle for console text color management

public:
	GameEngine(const int); // Constructor to initialize the game engine with a given number of players
	~GameEngine(); // Destructor
	GameEngine(const string) { cout << "Game Engine "; COPY_CTRT; }; // Copy constructor
	GameEngine operator = (GameEngine*) { cout << "Game Engine "; ASSIGN_OPRT; };  // Assignment (=) operator overload
	
	void buildBoard();    // Method to build the game board from a file
	const slotType getSlotType(const char); // Get the slot type based on a character representation
	void createPlayers(const int);  // Method to create players in the game
	void createDeck(); // Method to create the deck of cards

	void startGame(); // Method to start the game
	const bool turn(Player *); // Method to handle a player's turn
	const bool quitOrPlay(); // Method to handle the choice to quit or continue playing
	const int rollDice(); // Method to roll the dice and get a random number
	const bool assetOptions(Player*, Asset*); // Method to present options to a player when landing on an asset
	const bool instOptions(Player*, Instruction*); // Method to present options to a player when drawing an instruction
	const int drawFromDeck(); 
	Slot* movePlayer(Player*, const int);
	const bool choose_0_or_1(const int a = 0, const int b = 1); // Method to prompt a choice between two options (0 or 1)
	void boardPrint(); 
	list <Slot*>::iterator findSlot(const int); // Method to find a slot on the board by its position
	void owners(); // Method to print the owners of all assets on the board
	const bool nameCheck(const string); // Method to check if a player name is valid and unique
};
