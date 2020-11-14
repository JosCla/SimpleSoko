// game.cpp

// C++ Standard Library
#include <iostream>
using std::cout; using std::endl;
#include <vector>
using std::vector;
#include <fstream>
using std::ifstream;
#include <string>
using std::string; using std::getline;

// Libraries
#include <ncurses.h>

// Our files
#include "game.h"
#include "functions.h"
#include "color.h"

// Constructor
Game::Game() : ColorOn(false) {}

// Constructor (also loads data with filename)
Game::Game(const char *filename) : ColorOn(false) {
	LoadData(filename);
}

// Loads the data for a sokoban board given a filename
// Returns 0 for a successful read
// Returns 1 if file failed to open
// Returns 2 if file is improperly formatted
int Game::LoadData(const char *filename) {
	// Opening file
	ifstream fileIn;
	fileIn.open(filename);

	// If file failed to open, return 1
	if (fileIn.fail()) {
		return 1;
	}

	// Reading file header
	unsigned int numMetadata;
	fileIn >> LevelHeight >> LevelWidth >> numMetadata;
	
	// Reading main level
	Level.clear();
	for (unsigned int row = 0; row < LevelHeight; ++row) {
		// Getting the current line
		string currLine;
		getline(fileIn, currLine);
		// Seeing if the read failed
		if (fileIn.fail()) {
			return 2; // File is improperly formatted
		}
		// Splitting the current row into characters
		vector<char> currRow = splitToChar(currLine);
		// Making sure it isn't empty
		if (currRow.empty()) {
			--row;
			continue; // If it's empty, just try again with the next line
		}

		// Parsing the row
		for (unsigned int col = 0; col < currRow.size(); ++col) {
			char currChar = currRow.at(col);
			switch (currChar) {
				case '@':
					// Player
					currRow.at(col) = ' ';
				case '+':
					// Player on goal square
					if (currRow.at(col) != ' ')
						currRow.at(col) = '.';
					LevelPlayer.posX = col;
					LevelPlayer.posY = row;
					break;

				case '$':
					// Box
					currRow.at(col) = ' ';
				case '*':
					// Box on goal square
					if (currRow.at(col) != ' ')
						currRow.at(col) = '.';
					Box currBox;
					currBox.posX = col;
					currBox.posY = row;
					LevelBoxes.push_back(currBox);
					break;

				case '#':
				case ' ':
				case '.':
					// Valid characters
					break;

				default:
					// Invalid characters (converting to spaces)
					currRow.at(col) = ' ';
					break;
			}
		}

		// Expanding the row to fill the proper level width
		currRow.resize(LevelWidth, '.');

		// Pushing the current row into the level
		Level.push_back(currRow);
	}

	// Reading metadata
	LevelMetadata.clear();
	for (unsigned int i = 0; i < numMetadata; ++i) {
		// Getting the next line
		string currLine;
		getline(fileIn, currLine);
		// Checking to see if it failed
		if (fileIn.fail()) {
			return 2; // File is improperly formatted
		}
		// Checking to see if line is empty
		if (currLine.empty()) {
			--i;
			continue; // Try again with next line
		}

		// Parsing the metadata line
		long unsigned int colonInd = currLine.find(':');
		// Checking if metadata line is invalid
		if (colonInd == string::npos) {
			return 2; // File is improperly formatted
		}
		// Creating a Metadata object and putting in properties
		Metadata currData;
		currData.name = currLine.substr(0, colonInd);
		currData.value = currLine.substr(colonInd + 1);
		LevelMetadata.push_back(currData);
	}

	// Closing file
	fileIn.close();

	// Returning 0 to indicate successful read
	return 0;
}

// Toggles whether or not color is enabled
void Game::ToggleColor() {
	ColorOn = !ColorOn;
}

// Tries to move the player in a direction, returns whether or not it was successful
bool Game::MovePlayer(Direction dir) {
	// Getting new position
	int newPosX(LevelPlayer.posX), newPosY(LevelPlayer.posY);
	char moveChar = '0';
	switch (dir) {
		case DIR_LEFT:
			--newPosX;
			moveChar = 'l';
			break;

		case DIR_RIGHT:
			++newPosX;
			moveChar = 'r';
			break;

		case DIR_UP:
			--newPosY;
			moveChar = 'u';
			break;

		case DIR_DOWN:
			++newPosY;
			moveChar = 'd';
			break;

		default:
			break;
	}

	// Seeing if new position is in a wall
	if (Level.at(newPosY).at(newPosX) == '#') {
		return false;
	}

	// Seeing if a block needs to be moved
	bool canMove = true;
	int boxInd = BoxPresent(newPosX, newPosY);
	if (boxInd >= 0) {
		canMove = MoveBox(boxInd, dir);
		moveChar = toupper(moveChar);
	}
	
	// Moving the player if they can be moved
	if (canMove) {
		LevelPlayer.posX = newPosX;
		LevelPlayer.posY = newPosY;
		LevelMoves.push_back(moveChar);
	}

	return canMove;
}

// Tries to move a box in a direction, returns whether or not it was successful
bool Game::MoveBox(unsigned int ind, Direction dir) {
	Box currBox = LevelBoxes.at(ind);
	int newPosX(currBox.posX), newPosY(currBox.posY);
	switch (dir) {
		case DIR_LEFT:
			--newPosX;
			break;

		case DIR_RIGHT:
			++newPosX;
			break;

		case DIR_UP:
			--newPosY;
			break;

		case DIR_DOWN:
			++newPosY;
			break;

		default:
			break;
	}

	// Checking if new position is a wall
	if (Level.at(newPosY).at(newPosX) == '#') {
		return false;;
	}
	
	// Checking if new position is already occupied
	if (BoxPresent(newPosX, newPosY) >= 0) {
		return false;
	}
	
	// Updating box's position
	LevelBoxes.at(ind).posX = newPosX;
	LevelBoxes.at(ind).posY = newPosY;
	return true;
}

// Returns the index of the box present at a certain position, or -1 if no box present
int Game::BoxPresent(int posX, int posY) {
	for (unsigned int i = 0; i < LevelBoxes.size(); ++i) {
		Box currBox = LevelBoxes.at(i);
		if (currBox.posX == posX && currBox.posY == posY) {
			return i;
		}
	}
	return -1;
}

// Undos the player's last move
void Game::UndoMove() {
	// Returning if there are no moves to undo
	if (LevelMoves.empty()) {
		return;
	}

	// Getting the last move
	char lastMove = LevelMoves.back();
	LevelMoves.pop_back();

	// Undoing the last move
	int boxInd;
	switch (lastMove) {
		case 'L':
			boxInd = BoxPresent(LevelPlayer.posX - 1, LevelPlayer.posY);
			MoveBox(boxInd, DIR_RIGHT);
		case 'l':
			MovePlayer(DIR_RIGHT);
			break;

		case 'R':
			boxInd = BoxPresent(LevelPlayer.posX + 1, LevelPlayer.posY);
			MoveBox(boxInd, DIR_LEFT);
		case 'r':
			MovePlayer(DIR_LEFT);
			break;

		case 'U':
			boxInd = BoxPresent(LevelPlayer.posX, LevelPlayer.posY - 1);
			MoveBox(boxInd, DIR_DOWN);
		case 'u':
			MovePlayer(DIR_DOWN);
			break;

		case 'D':
			boxInd = BoxPresent(LevelPlayer.posX, LevelPlayer.posY + 1);
			MoveBox(boxInd, DIR_UP);
		case 'd':
			MovePlayer(DIR_UP);
			break;
	}

	// (popping another move to account for movement)
	LevelMoves.pop_back();
}

// Draws the game board
void Game::DrawBoard(WINDOW *win) {
	// Adding the base board
	for (unsigned int i = 0; i < LevelHeight; ++i) {
		for (unsigned int j = 0; j < LevelWidth; ++j) {
			mvwaddch(win, i + 1, j + 1, Level.at(i).at(j));
			if (ColorOn) {
				switch(Level.at(i).at(j)) {
					case '.':
						mvwchgat(win, i + 1, j + 1, 1, A_NORMAL, COLOR_TARGET, NULL);
						break;
					case '#':
						mvwchgat(win, i + 1, j + 1, 1, A_NORMAL, COLOR_WALL, NULL);
						break;
				}
			}
		}
	}
	
	// Adding the player
	wmove(win, LevelPlayer.posY + 1, LevelPlayer.posX + 1);
	if (Level.at(LevelPlayer.posY).at(LevelPlayer.posX) != '.') {
		waddch(win, '@');
		if (ColorOn)
			mvwchgat(win, LevelPlayer.posY + 1, LevelPlayer.posX + 1, 1, A_NORMAL, COLOR_PLAYER, NULL);
	} else {
		waddch(win, '+');
		if (ColorOn)
			mvwchgat(win, LevelPlayer.posY + 1, LevelPlayer.posX + 1, 1, A_NORMAL, COLOR_PLAYER_TARGET, NULL);
	}

	// Adding the boxes
	for (unsigned int i = 0; i < LevelBoxes.size(); ++i) {
		Box currBox = LevelBoxes.at(i);
		wmove(win, currBox.posY + 1, currBox.posX + 1);
		if (Level.at(currBox.posY).at(currBox.posX) != '.') {
			waddch(win, '$');
			if (ColorOn)
				mvwchgat(win, currBox.posY + 1, currBox.posX + 1, 1, A_NORMAL, COLOR_BOX, NULL);
		} else {
			waddch(win, '*');
			if (ColorOn)
				mvwchgat(win, currBox.posY + 1, currBox.posX + 1, 1, A_NORMAL, COLOR_BOX_TARGET, NULL);
		}
	}
}

// Returns whether the game has been won or not
bool Game::CheckWin() {
	bool isWin = true;
	for (unsigned int i = 0; i < LevelBoxes.size(); ++i) {
		Box currBox = LevelBoxes.at(i);
		if (Level.at(currBox.posY).at(currBox.posX) != '.') {
			isWin = false;
		}
	}
	return isWin;
}

// A function for getting the metadata
vector<Metadata> Game::GetMetadata() {
	return LevelMetadata;
}
