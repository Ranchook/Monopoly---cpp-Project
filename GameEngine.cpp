#include "gameEngine.h"

// Constructor for GameEngine, initializes game components and starts the game
GameEngine::GameEngine(const int playersNum)
{
	buildBoard();	
	createPlayers(playersNum);
	createDeck();

	// Setting color in command window for better readability
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	startGame();
}

// Destructor for GameEngine, cleans up dynamic memory
GameEngine::~GameEngine()
{
	players.clear();
	while (!deck.empty())
		deck.pop();	
	board.clear();
}

// Method to build the game board from a file
void GameEngine::buildBoard()
{
	// Buffer for reading lines from the file
	char buff[40];
	int startFlag = 0;
	ifstream gameBoard(BOARD, ios::in);  // Open the board file
	gameBoard.seekg(0, ios::beg); // Set the position to the beginning of the file
	try {
		
		if (!gameBoard.is_open()) throw "file not opened";
		
		while (!gameBoard.eof())
		{
			gameBoard.getline(buff, 40, ','); // Read a line from the file
			if (*buff == 'I') // If the slot is an Instruction
			{
				gameBoard.getline(buff, 40, ','); // Read the instruction type
				char instType = *buff;
				
				// Check if start slot already exist
				if (startFlag == 1 && instType == 'S') BAD_INPUT;
				if (*buff == 'S') 
					startFlag = 1;
				
				gameBoard.getline(buff, 40); //Read the rest of line from file
				string instName = buff; // Save the instruction name
				Instruction *temp = new Instruction(instName, getSlotType(instType));
				//board.push_front(temp);
				board.push_back(temp); // Add the instruction to the board
			}
			
			else if (*buff == 'P') // If the slot is an Asset
			{
				gameBoard.getline(buff, 40, ','); // Read the city name
				string cityName = buff;
				gameBoard.getline(buff, 40, ','); // Read the asset name
				string assetName = buff;
				gameBoard.getline(buff, 40, ','); // Read the house price
				int housePrice = atoi(buff);
				gameBoard.getline(buff, 40); // Read the rent price
				int rentPrice = atoi(buff);
				Asset *temp = new Asset(assetName, cityName, housePrice, rentPrice);
				
				board.push_back(temp); // Add the asset to the board
				
				//board.push_front(temp); // putting slot to board
			}
			else if (*buff == NULL) // If end of file
				break;
			else throw "File not valid!\n"; // Throw an error if file format is invalid
			slotTotal++; // Increment slot total
		}
	}
	catch (const char* err)
	{cout << err;} // Print error message

	gameBoard.close(); // Close the file
	

	//for Debug
	//boardPrint();
}

// Method to get the slot type based on a character representation
const slotType GameEngine::getSlotType(const char c)
{
	// If landed on Instruction slot, return Instruction type
	if (c == 'S') return Start; 
	if (c == 'J') return Jail;
	return Ticket;
}

// Method to create players
void GameEngine::createPlayers(const int total)
{
	// Assign different color for each player
	int colorIndx = 9;
	for (int i = 0; i < total; i++, colorIndx++)
	{
		if (colorIndx == 16)
			colorIndx = 9;
		cout << "Player " << i + 1 << " name: ";
		string name;
		cin >> name; 
		// If name repeats, re-enter name
		while (!nameCheck(name))
			cin >> name; 
		Player *temp = new Player(name, colorIndx); // Create new player
		players.push_back(temp); // Add player to the end of players line
	}
}

// Method to create the deck of cards
void GameEngine::createDeck()
{
	// Create  deck of pre-determined number of cards with random values between (-350) to 350
	srand((unsigned int)time(NULL));
	for (int i = 0; i < CARDS; i++)
		deck.push(rand()% 700-350);
	drawFromDeck();
}

// Method to start the game
void GameEngine::startGame()
{
	// Point to first player
	deque <Player*>::iterator playerTurn = players.begin();
	
	// Play while there is more than one player
	while (players.size() > 1)
	{
		// Printing player turn in their  color
		SetConsoleTextAttribute(hConsole, (*playerTurn)->color);
		cout << "\n========================================\n";
		cout << "It's " << (*playerTurn)->getPlayerName() << "'s turn" << endl;
		cout << "you have " << (*playerTurn)->getMoney() << " NIS" << endl;
		if ((*playerTurn)->getJail())
		{
			// If player is in jail, release him and skip his turn
			cout << "Your'e in jail, you'll play next turn" << endl;
			(*playerTurn)->setJail(false);
			if (playerTurn != players.end() - 1)
				playerTurn++;
			else
				playerTurn = players.begin();
			continue;
		}

		//if player lost/quit remove him from the vector
		if (!turn(*playerTurn))
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

		// Check if it's the last player's turn
		if (playerTurn == players.end() - 1)
		{
			// If more than 1 players, print each player's assets
			if (players.size() > 1)
				owners();
			playerTurn = players.begin();
			continue;
		}
		playerTurn++;
	}

	// Print winner in "formal" white color
	WHITE_PRINT;	// print winner name in white
	cout << "\nThe winner is:" << (*playerTurn)->getPlayerName() << endl << endl;
	delete *playerTurn;
}

// Method to handle a player's turn
const bool GameEngine::turn(Player *p)
{
	if (!quitOrPlay())
	{
		WHITE_PRINT;
		cout << p->getPlayerName() << " just quit the game, what a loser!\n" << endl;
		return false;
	}
	cout << endl << p->getPlayerName() << " is Currently in slot " << p->getPosition();
	Slot* temp = movePlayer(p, rollDice()); // Move player and save new slot
	cout << p->getPlayerName() << " is now in slot " << p->getPosition() << endl;
	temp->printSlot();
	
	// Act according to slot type
	if (ASST(temp))
		return assetOptions(p, ASST(temp));
	else
		return instOptions(p, INST (temp)); 
}

// Method to handle the choice to quit or continue playing
const bool GameEngine::quitOrPlay()
{
	cout << "Please choose if you'd like to keep playing or quit:" << endl;
	cout << "0 - quit. 1 - keep playing: ";
	return choose_0_or_1();
}

// Method to roll the dice and get a random number
const int GameEngine::rollDice()
{
	// Returns value between 1 to 6
	srand((unsigned int)time(NULL));
	int temp =  ((rand() % 6) + 1);
	cout << " Rolled dice gave - " << temp << endl;
	return temp;
}

// Method to move a player a certain number of slots
Slot* GameEngine::movePlayer(Player *p, const  int dice)
{
	// Pointing to last slot
	list <Slot*>::iterator lastSlot = findSlot(slotTotal);
	
	// Find player current position
	list <Slot*>::iterator iter = findSlot(p->getPosition());
	
	for (int steps = 0; steps < dice; steps++)
	{
		// If player completes a lap, point to slot 1
		if (iter == lastSlot)
		{
			iter = board.begin();
			// If player has no more steps stay on slot 1
			if (dice - steps > 1)
			{
				cout << "Passed on Start, get " << START_SUM << " NIS\n";
				instOptions(p, INST(*iter)); // Adding START_SUM to player
			}
			continue;
		}
		iter++;
	}
	p->setPosition((*iter)->getSlotNum()); // Update player position
	return (*iter);
}

// Method to present options to a player when they land on an asset slot
const bool GameEngine::assetOptions(Player *p, Asset* house)
{
	if (!house->getOwner()) // If house is not owned
	{
		// Check if player have enough money to buy it		
		if (p->getMoney() < house->getHousePrice()) 
			cout << "You don't have enough money to buy this house!\n";	
		else	// Player have enough money to buy it
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
	else // House is owned by someone
	{
		// Check if self owned
		if(house->getOwner() == p)
		cout << "This is your asset" << endl;
		else
		{
			int rentPrice = house->getRent();
			cout << "This is " << house->getOwner()->getPlayerName() << "'s asset, ";
			cout << "you need to pay " << rentPrice << " NIS\n";
			if (!house->isMortg()) // If house is not mortgage, pay to owner
				return p->payment(-rentPrice, house->getOwner());
			else	// House is mortgaged, owner doesn't get paid
				cout << "Asset is mortgaged! paying to bank \n";
			return p->payment(-rentPrice); // Player pays rent fee
		}
	}
	return true;
}

// Method of player actions when they land on an instruction slot
const bool GameEngine::instOptions(Player *p, Instruction *inst)
{
	slotType temp = inst->getSlotType();
	if (temp == Start) // Landed on "Start" slot
	{
		p->payment(START_SUM); // Add 350 to money and free mortgaged assets
		cout << "\nYou now have " << p->getMoney() << " NIS\n\n";
		p->findMortgage();	// Add mortgage years
	}
	else if (temp == Ticket) // Landed on "Get Ticket" slot
	{
		int ticket = drawFromDeck(); // Draw card from deck
		cout << "You drew (" << ticket << ") from tickets deck\n";
		return (p->payment(ticket));
	}
	else // Landed in Jail
	{
		cout << "You are in jail, you won't play your next turn!\n";
		p->setJail(true); // Prevents player to play next turn
	}
	return true;
}

// Method to draw a card from the deck
const int GameEngine::drawFromDeck()
{
	int temp = deck.front(); // Save the top deck value
	deck.pop(); // Remove from top of deck
	deck.push(temp);// Place value at the bottom of the deck
	return temp;
}

// Method to choose between two options (0 or 1)
const bool GameEngine::choose_0_or_1(int option1, int option2)
{
	int sel;
	cin >> sel;
	while (cin.fail() || sel != option1 && sel != option2)
	{
		// If entered out of range or chars, enter new selection
		cout << "please choose " << option1 << " or " << option2 << " :";
		
		// Clean buffer
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin >> sel;
	} 
	return sel;
}

// Rough print for personal use
void GameEngine::boardPrint()
{
	// Rough print of board for DEBUG
	cout << "The board is (after last slot you return to slot 1): " << endl;
	list <Slot*>::iterator iter = board.begin();
	for (iter; iter != board.end(); iter++)
		cout << (*iter)->getSlotNum() << " " << (*iter)->getSlotName() << endl;
	cout << "\n========================================\n";
}

// Method to find a specific slot
list<Slot*>::iterator GameEngine::findSlot(const int indx)
{
	list <Slot*>::iterator iter = board.begin();
	for (int i = 1; i < indx; i++)
		iter++;
	return iter;
}

// Method to print all assets that each player owns
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

// Method to check if a player name is unique
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
