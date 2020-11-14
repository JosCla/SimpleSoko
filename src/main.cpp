// main.cpp

// C++ Standard Library
#include <iostream> // input/output
using std::cout; using std::endl; using std::cerr;
#include <vector> // vectors
using std::vector;
#include <string> // strings
using std::string;

// Libraries
#include <ncurses.h> // ncurses library

// Our files
#include "game.h" // game-related classes
#include "color.h" // color setup
#include "ui.h" // user interface

int main(int argc, char *argv[]) {
	// Checking to see if we've received the correct number of arguments
	if (argc != 2) {
		cerr << "Please enter 1 argument for the level filepath" << endl;
		return 1;
	}

	// Initializing sokoban map
	char *filepath = argv[1];
	Game ourGame;
	int loadSuccess = ourGame.LoadData(filepath);
	if (loadSuccess != 0) {
		cerr << "File failed to open with code " << loadSuccess << ". Aborting." << endl;
		return 1;
	}

	// Initializing ncurses
	initscr();
	cbreak(); // Take input character-by-character
	curs_set(0); // Disable cursor displaying
	keypad(stdscr, true); // Enable special inputs
	noecho(); // Disable input echoing
	refresh(); // Refresh screen

	// Turning on color (if we're able to)
	if (has_colors()) {
		ourGame.ToggleColor();
		start_color();
		setupColor();
	}

	// Getting UI ready
	UI ourUI;
	ourUI.AllocateWindows();
	ourUI.DrawGameWin(ourGame);
	ourUI.DrawDataWin(ourGame.GetMetadata());
	ourUI.DrawControlWin();

	// Main game loop
	int exitGame = 0;
	while (exitGame == 0) {
		int userInput = getch();
		switch (userInput) {
			case KEY_LEFT:
				ourGame.MovePlayer(DIR_LEFT);
				break;

			case KEY_RIGHT:
				ourGame.MovePlayer(DIR_RIGHT);
				break;

			case KEY_UP:
				ourGame.MovePlayer(DIR_UP);
				break;

			case KEY_DOWN:
				ourGame.MovePlayer(DIR_DOWN);
				break;

			case 'z':
				ourGame.UndoMove();
				break;

			case 'q':
				exitGame = 2;
				break;

			default:
				break;
		}

		if (ourGame.CheckWin()) {
			exitGame = 1;
		}

		ourUI.DrawGameWin(ourGame);
	}
	
	// Giving a message before exiting
	if (exitGame == 1) {
		mvprintw(0, 1, "You win! Yay!");
	} else if (exitGame == 2) {
		mvprintw(0, 1, "Exited game.");
	}
	refresh();

	// Exiting on input
	getch(); // Waiting for input
	endwin(); // Exit ncurses mode

	return 0;
}
