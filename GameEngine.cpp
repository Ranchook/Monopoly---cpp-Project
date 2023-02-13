#include "gameEngine.h"

GameEngine::GameEngine(const int playersNum)
{
	buildBoard();
	createPlayers(playersNum);
	createDeck();

	//setting color in cmd window for comfort
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	startGame();
}

GameEngine::~GameEngine()
{
	players.clear();
	while (!deck.empty())
		deck.pop();	
	board.clear();
}

void GameEngine::buildBoard()
{
	//reading file and creating board
	char buff[40];
	int startFlag = 0;
	ifstream gameBoard(BOARD, ios::in);
	gameBoard.seekg(0, ios::beg);
	try {
		
		if (!gameBoard.is_open()) throw "file not opened";
		
		while (!gameBoard.eof())
		{
			gameBoard.getline(buff, 40, ',');
			if (*buff == 'I')
			{
				gameBoard.getline(buff, 40, ',');
				char instType = *buff;
				// checking if start slot already exist
				if (startFlag == 1 && instType == 'S') BAD_INPUT;
				if (*buff == 'S') 
					startFlag = 1;
				gameBoard.getline(buff, 40); //reading til end of line from file
				string instName = buff; // saving slot name
				Instruction *temp = new Instruction(instName, getSlotType(instType));
				//board.push_front(temp); // putting slot to board
				board.push_back(temp);
			}
			
			else if (*buff == 'P')
			{
				gameBoard.getline(buff, 40, ',');
				string cityName = buff;
				gameBoard.getline(buff, 40, ',');
				string assetName = buff;
				gameBoard.getline(buff, 40, ',');
				int housePrice = atoi(buff);	//using atoi to read integers
				gameBoard.getline(buff, 40);
				int rentPrice = atoi(buff);
				Asset *temp = new Asset(assetName, cityName, housePrice, rentPrice);
				
				board.push_back(temp);
				
				//board.push_front(temp); // putting slot to board
			}
			else if (*buff == NULL) // if end of file
				break;
			else throw "File not valid!\n";
			slotTotal++; //saving slot total
		}
	}
	catch (const char* err)
	{cout << err;}

	gameBoard.close();
	

	//for Debug
	//boardPrint();
}

const slotType GameEngine::getSlotType(const char c)
{
	//if landed on Instruction slot, return Instruction type
	if (c == 'S') return Start;
	if (c == 'J') return Jail;
	return Ticket;
}

void GameEngine::createPlayers(const int total)
{
	//assign different color for each player
	int colorIndx = 9;
	for (int i = 0; i < total; i++, colorIndx++)
	{
		if (colorIndx == 16)
			colorIndx = 9;
		cout << "Player " << i + 1 << " name: ";
		string name;
		cin >> name; 
		//if name repeats, re-enter name
		while (!nameCheck(name))
			cin >> name; 
		Player *temp = new Player(name, colorIndx); // creating new player
		players.push_back(temp); // adding him to the end of players line
	}
}

void GameEngine::createDeck()
{
	// creating deck of pre-determined number of cards 
	// with values between (-350) to 350
	srand((unsigned int)time(NULL));
	for (int i = 0; i < CARDS; i++)
		deck.push(rand()% 700-350);
	drawFromDeck();
}

void GameEngine::startGame()
{
	//point to first player
	deque <Player*>::iterator playerTurn = players.begin();
	// play while last player are different players
	while (players.size() > 1)
	{
		// printing player turn in his color
		SetConsoleTextAttribute(hConsole, (*playerTurn)->color);
		cout << "\n========================================\n";
		cout << "It's " << (*playerTurn)->getPlayerName() << "'s turn" << endl;
		cout << "you have " << (*playerTurn)->getMoney() << " NIS" << endl;
		if ((*playerTurn)->getJail())
		{
			//if player is in jail, release him and skip his turn
			cout << "Your'e in jail, you'll play next turn" << endl;
			(*playerTurn)->setJail(false);
			if (playerTurn != players.end() - 1)
				playerTurn++;
			else
				playerTurn = players.begin();
			continue;
		}

		if (!turn(*playerTurn)) //if player lost/quit remove him from the vector
		{
			WHITE_PRINT;
			if (playerTurn == players.end() - 1)
			{
				delete *playerTurn;
				playerTurn = players.erase(playerTurn);
				playerTurn = players.begin();
				continue;
			}
			delete *playerTurn;
			playerTurn = players.erase(playerTurn);
			continue;
		}

		cout << (*playerTurn)->getPlayerName() << "'s turn is over" << endl;
		cout << "you now have " << (*playerTurn)->getMoney() << " NIS" << endl;

		//check if last player turn
		if (playerTurn == players.end() - 1)
		{
			//if more than 1 players, print each player's assets
			if (players.size() > 1)
				owners();
			playerTurn = players.begin();
			continue;
		}
		playerTurn++;
	}

	//print winner in "formal" white color
	WHITE_PRINT;	// print winner name in white
	cout << "\nThe winner is:" << (*playerTurn)->getPlayerName() << endl << endl;
	delete *playerTurn;
}

const bool GameEngine::turn(Player *p)
{
	if (!quitOrPlay())
	{
		WHITE_PRINT;
		cout << p->getPlayerName() << " just quit the game, what a loser!\n" << endl;
		return false;
	}
	cout << endl << p->getPlayerName() << " is Currently in slot " << p->getPosition();
	Slot* temp = movePlayer(p, rollDice()); // moving player and saving new slot
	cout << p->getPlayerName() << " is now in slot " << p->getPosition() << endl;
	temp->printSlot();
	
	//act according to slot type
	if (ASST(temp))
		return assetOptions(p, ASST(temp));
	else
		return instOptions(p, INST (temp)); 
}

const bool GameEngine::quitOrPlay()
{
	cout << "Please choose if you'd like to keep playing or quit:" << endl;
	cout << "0 - quit. 1 - keep playing: ";
	return choose_0_or_1();
}

const int GameEngine::rollDice()
{
	//returns value between 1 to 6
	srand((unsigned int)time(NULL));
	int temp =  ((rand() % 6) + 1);
	cout << " Rolled dice gave - " << temp << endl;
	return temp;
}

Slot* GameEngine::movePlayer(Player *p, const  int dice)
{
	//pointing to last slot
	list <Slot*>::iterator lastSlot = findSlot(slotTotal);
	
	//find player current position
	list <Slot*>::iterator iter = findSlot(p->getPosition());
	
	for (int steps = 0; steps < dice; steps++)
	{
		// if player completes a lap, point to slot 1
		if (iter == lastSlot)
		{
			iter = board.begin();
			//if player has no more steps stay on slot 1
			if (dice - steps > 1)
			{
				cout << "Passed on Start, get " << START_SUM << " NIS\n";
				instOptions(p, INST(*iter)); // adding START_SUM to player
			}
			continue;
		}
		iter++;
	}
	p->setPosition((*iter)->getSlotNum()); // update player position
	return (*iter);
}

const bool GameEngine::assetOptions(Player *p, Asset* house)
{
	if (!house->getOwner()) //if house is not owned
	{
		// check if player have enough money to buy it		
		if (p->getMoney() < house->getHousePrice()) 
			cout << "You don't have enough money to buy this house!\n";	
		else	//player have enough money to buy it
		{
			cout << "Would you like to buy this house?" << endl;
			cout << "1 - Yes. 0 - No" << endl;
			if (choose_0_or_1())
			{
				p->buyAsset(house);
				cout << p->getPlayerName() << " just bought:\n";
				house->printSlot();
			}
		}
		
	}
	else // house is owned by someone
	{
		//check if self owned
		if(house->getOwner() == p)
		cout << "This is your asset" << endl;
		else
		{
			int rentPrice = house->getRent();
			cout << "This is " << house->getOwner()->getPlayerName() << "'s asset, ";
			cout << "you need to pay " << rentPrice << " NIS\n";
			if (!house->isMortg()) // if house is not mortgage, pay to owner
				return p->payment(-rentPrice, house->getOwner());
			else// house is mortgaged, owner doesn't get paid
				cout << "Asset is mortgaged! paying to bank \n";
			return p->payment(-rentPrice); // player pays rent fee
		}
	}
	return true;
}

const bool GameEngine::instOptions(Player *p, Instruction *inst)
{
	slotType temp = inst->getSlotType();
	if (temp == Start) //landed on "Start" slot
	{
		p->payment(START_SUM); // add 350 to money and free mortgaged assets
		cout << "\nYou now have " << p->getMoney() << " NIS\n\n";
		p->findMortgage();		//add mortgage years
	}
	else if (temp == Ticket) //landed on "Get Ticket" slot
	{
		int ticket = drawFromDeck(); // draw card from deck
		cout << "You drew (" << ticket << ") from tickets deck\n";
		return (p->payment(ticket));
	}
	else //landed in Jail
	{
		cout << "You are in jail, you won't play your next turn!\n";
		p->setJail(true); // prevents player to play next turn
	}
	return true;
}

const int GameEngine::drawFromDeck()
{
	int temp = deck.front(); // saving top deck value
	deck.pop(); // removing from top of deck
	deck.push(temp);//placing value at the bottom of the deck
	return temp;
}

//choosing between two options
const bool GameEngine::choose_0_or_1(int option1, int option2)
{
	int sel;
	cin >> sel;
	while (cin.fail() || sel != option1 && sel != option2)
	{
		// if entered out of range or chars, enter new selection
		cout << "please choose " << option1 << " or " << option2 << " :";
		
		//clean buffer
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin >> sel;
	} 
	return sel;
}

//rough print for personal use
void GameEngine::boardPrint()
{
	//rough print of board for DEBUG
	cout << "The board is (after last slot you return to slot 1): " << endl;
	list <Slot*>::iterator iter = board.begin();
	for (iter; iter != board.end(); iter++)
		cout << (*iter)->getSlotNum() << " " << (*iter)->getSlotName() << endl;
	cout << "\n========================================\n";
}

//finding specific slot
list<Slot*>::iterator GameEngine::findSlot(const int indx)
{
	list <Slot*>::iterator iter = board.begin();
	for (int i = 1; i < indx; i++)
		iter++;
	return iter;
}

//print all assets that each player own
void GameEngine::owners()
{
	WHITE_PRINT;
	cout << "\n*****************************************\n";
	deque <Player*>::iterator iter = players.begin();
	for (iter; iter != players.end(); iter++)
	{
		cout << (*iter)->getPlayerName() << " now owns:\n";
		(*iter)->getHouses();
	}
	cout << "\n*****************************************\n";
}

//check if repeated name
const bool GameEngine::nameCheck(const string str)
{
	deque <Player*>::iterator iter = players.begin();
	for (iter; iter != players.end(); iter++)
	{
		if ((*iter)->getPlayerName() == str)
		{
			cout << "Entered same name! Please enter different name: ";
			return false;
		}
	}
	return true;
}