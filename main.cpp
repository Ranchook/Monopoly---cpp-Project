#include "GameEngine.h"

using namespace std;

int main()
{
	int playersNum;
	cout << "Welcome to Monopoly!\n";
	cout << "Please enter number of players: ";
	try {
		cin >> playersNum;
		if (cin.fail())
			BAD_INPUT;
		if (playersNum < 2)
			throw "Not Enough Players! exiting...\n";
	
		GameEngine *temp = new GameEngine(playersNum);
		temp->~GameEngine();
	}
	catch (const char* err)
	{
		cout << err;
	}
	system("pause");
}