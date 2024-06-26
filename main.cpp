#include "GameEngine.h"

using namespace std;

int main()
{
	int playersNum; // Variable to store the number of players
	cout << "Welcome to Monopoly!\n";
	cout << "Please enter number of players: ";
	try {
		cin >> playersNum; // Input the number of players
		if (cin.fail())
			BAD_INPUT;
		if (playersNum < 2) // Check if the number of players is less than 2
			throw "Not Enough Players! exiting...\n";
	
		GameEngine *temp = new GameEngine(playersNum);  // Create a new GameEngine instance with the specified number of players
		temp->~GameEngine(); // Manually call the destructor to clean up
	}
	catch (const char* err)
	{
		cout << err; // Catch and display any errors
	}
	system("pause");
}
