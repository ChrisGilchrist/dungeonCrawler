#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

vector<vector<char>> map;
int goldCount = 0;

auto startTime = chrono::system_clock::now();
auto endTime = chrono::system_clock::now();

// Important symbols in the game
const char PlayerSymbol = 'P';
const char ExitSymbol = 'E';
const char MonsterSymbol = 'M';
const char GoldSymbol = 'G';
const char EmptySymbol = '.';
const char WallSymbol = '#';

enum Keys
{
	Right = 'd',
	Left = 'a',
	Up = 'w',
	Down = 's',
	Quit = 'q'
};

enum GameState
{
	HitMonster,
	FoundExit,
	QuitGame,
	StillLooking
};

void createMap() {
	string STRING;
	ifstream infile;

	//  Opens file
	infile.open("map.txt");

	// If the file opens correctly then start looping
	if (infile.is_open()) {

		// Loops through each line and prints it to the console
		while (getline(infile, STRING)) {

			vector<char> row; // Create a new empty row

			// Get each character from the line in the file
			vector<char> v(STRING.begin(), STRING.end());

			// Push each character into the row, each item making a new column
			for (char& currChar : v) {
				row.push_back(currChar);
			}

			// Push the new row to the map vector
			map.push_back(row);

			cout << STRING << '\n';
		}

		// When there is no more lines, it will close the file
		infile.close();
	}
}

void printMap() {
	for (int y = 0; y < map.size(); ++y) {

		for (int x = 0; x < map[0].size(); ++x) {
			cout << map[y][x];
		}
		cout << '\n';
	}
}

void moveMonster(
	int& currentPosX, int& currentPosY,
	int desiredPosX, int desiredPosY) {
	


}

GameState movePlayer(
					 int & currentPosX, int & currentPosY,
					 int desiredPosX, int desiredPosY) {
	
	// First Check the user is not out of the map
	if (desiredPosX < 0) { desiredPosX = 0; }
	if (desiredPosY < 0) { desiredPosY = 0; }

	// Check that their next move does not make them go out the map
	if (desiredPosX == 0 || desiredPosX == map.size() - 1) { desiredPosX = currentPosX; };
	if (desiredPosY == 0 || desiredPosY == map[0].size() - 1) { desiredPosY = currentPosY; };

	GameState newGameState;

	if (map[desiredPosX][desiredPosY] == MonsterSymbol) {
		cout << "GAME OVER - A monster got you first!" << '\n';
		cout << "Gold Count: " << goldCount << '\n';
		newGameState = HitMonster;
	}
	else if (map[desiredPosX][desiredPosY] == ExitSymbol) {
		endTime = chrono::system_clock::now();
		cout << "CONGRATULATIONS - You successfully made it to the exit unharmed!" << '\n';

		// Show stats
		chrono::duration<double> elapsed_seconds = endTime - startTime;
		cout << "Gold Count: " << goldCount << '\n';
		cout << "Elapsed Time: " << elapsed_seconds.count() << '\n';
		

		newGameState = FoundExit;
	}
	else if (map[desiredPosX][desiredPosY] == GoldSymbol) {
		goldCount = goldCount + 1;
		cout << "GOLD FOUND - You have found some treasure on your journey!" << " Gold Count: " << goldCount << '\n';
		newGameState = StillLooking;
	}
	else if (map[desiredPosX][desiredPosY] == WallSymbol) {
		desiredPosX = currentPosX;
		desiredPosY = currentPosY;
		newGameState = StillLooking;
	}

	else {
		newGameState = StillLooking;
	}

	// Clear current position
	map[currentPosX][currentPosY] = EmptySymbol;

	// Place the player symbol
	map[desiredPosX][desiredPosY] = PlayerSymbol;

	currentPosX = desiredPosX;
	currentPosY = desiredPosY;
	return newGameState;
}

int main() {
	int playerPosX = 7;
	int playerPosY = 3;

	GameState gameState = StillLooking;

	// Set the start time
	startTime = chrono::system_clock::now();

	// Load in the map
	createMap();

	/* In the vector, find where the current start location of the player is by looking for 'S' */
	for (int x = 0; x < map.size(); ++x) {

		for (int y = 0; y < map[0].size(); ++y) {

			if (map[x][y] == PlayerSymbol) {
				playerPosX = x;
				playerPosY = y;
				printMap();
			}
		}
	} 

	// While we are still looking, listen for key presses from user
	while (gameState == StillLooking) {

		char key;
		cin >> key;

		if (key == Right) {
			gameState = movePlayer(playerPosX, playerPosY, playerPosX, playerPosY + 1);
			// cout << "Moved character to the right by 1" << '\n';
		}
		else if (key == Left) {
			gameState = movePlayer(playerPosX, playerPosY, playerPosX, playerPosY - 1);
			// cout << "Moved character to the left by 1" << '\n';
		}
		else if (key == Up) {
			gameState = movePlayer(playerPosX, playerPosY, playerPosX - 1, playerPosY);
			// cout << "Moved character up by 1" << '\n';
		}
		else if (key == Down) {
			gameState = movePlayer(playerPosX, playerPosY, playerPosX + 1, playerPosY);
			// cout << "Moved character down by 1" << '\n';
		}
		else if (key == Quit) {
			gameState = QuitGame;
			cout << "Player has quit the game" << '\n';
			exit(0);
		}
		else
		{
			cout << "Invalid control, please try again" << '\n';
		}
		
		// Generate a random direcition for the monster to move in
		// moveMonster();

		printMap();

	}

	return 0;
}