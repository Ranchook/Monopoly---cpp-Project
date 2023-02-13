#pragma once
#include "Player.h"
#include "Instruction.h"
#include <ctime>
#include <fstream>
#include <list>
#include <queue>
#include <deque>
#include <windows.h>

#undef max // because of windows.h => used in buffer clean
			// in line 328 @ gameEngine.cpp

#define BOARD "board.txt"
#define CARDS 20
#define ASST dynamic_cast <Asset*> 
#define INST dynamic_cast <Instruction*> 
#define WHITE_PRINT SetConsoleTextAttribute(hConsole, 15)

class GameEngine
{
	deque <Player*> players;
	queue <int> deck;
	list <Slot*> board;
	int slotTotal = 0;
	//color variation using HANDLE class
	HANDLE hConsole;

public:
	GameEngine(const int);
	~GameEngine();
	GameEngine(const string) { cout << "Game Engine "; COPY_CTRT; }; // copy constructor
	GameEngine operator = (GameEngine*) { cout << "Game Engine "; ASSIGN_OPRT; };
	

	void buildBoard();
	const slotType getSlotType(const char);
	void createPlayers(const int);
	void createDeck();

	void startGame();
	const bool turn(Player *);
	const bool quitOrPlay();
	const int rollDice();
	const bool assetOptions(Player*, Asset*);
	const bool instOptions(Player*, Instruction*);
	const int drawFromDeck();
	Slot* movePlayer(Player*, const int);
	const bool choose_0_or_1(const int a = 0, const int b = 1);
	void boardPrint();
	list <Slot*>::iterator findSlot(const int);
	void owners();
	const bool nameCheck(const string);

};