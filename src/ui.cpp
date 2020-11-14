// ui.cpp

// C++ Standard Library
#include <vector>
using std::vector;
#include <string>
using std::string;

// Libraries
#include <ncurses.h>

// Our files
#include "game.h"
#include "ui.h"
#include "functions.h"

// Constructor
UI::UI() {}

// Constructor
UI::UI(WINDOW *gameWin, WINDOW *dataWin, WINDOW *controlWin) {
	GameWin = gameWin;
	DataWin = dataWin;
	ControlWin = controlWin;
}

// Correctly allocates space for each window at any screen size
void UI::AllocateWindows() {
	// Finding screen size
	int maxY, maxX;
	getmaxyx(stdscr, maxY, maxX);

	// Allocating the left half of the screen to the game window
	GameWin = newwin(maxY, maxX / 2, 0, 0);

	// Allocating the right side of the screen to the data window,
	// excluding the bottom 7 rows
	DataWin = newwin(maxY - 7, maxX - (maxX / 2), 0, maxX / 2);

	// Allocating the bottom 7 rows of the right side to the controls window
	ControlWin = newwin(7, maxX - (maxX / 2), maxY - 7, maxX / 2);
}

// Draws the game window
void UI::DrawGameWin(Game &game) {
	// Clearing the window
	wclear(GameWin);

	// Drawing the window's border
	box(GameWin, 0, 0);
	mvwprintw(GameWin, 0, 1, "SimpleSoko");

	// Drawing the window's contents
	game.DrawBoard(GameWin);

	// Refreshing the window
	wrefresh(GameWin);
}

void UI::DrawDataWin(const vector<Metadata> &data) {
	// Clearing the window
	wclear(DataWin);

	// Drawing the window's border
	box(DataWin, 0, 0);
	mvwprintw(DataWin, 0, 1, "Level Data");

	// Getting window dimensions for printing properties
	unsigned int maxY, maxX;
	getmaxyx(DataWin, maxY, maxX);

	// Drawing the window's contents
	unsigned int currLine = 1;
	for (unsigned int i = 0; i < data.size(); ++i) {
		string currData = data.at(i).name + (string)": " + data.at(i).value;
		vector<string> currDataSplit = splitToLines(currData, maxX - 2);

		for (unsigned int j = 0; j < currDataSplit.size(); ++j) {
			mvwprintw(DataWin, currLine, 1, "%s", currDataSplit.at(j).c_str());
			++currLine;
			if (currLine > maxY - 2) {
				break;
			}
		}
		mvwchgat(DataWin, currLine - currDataSplit.size(), 1, data.at(i).name.length(), A_BOLD, 0, NULL);

		if (currLine > maxY - 2) {
			break;
		}
	}

	// Refreshing the window
	wrefresh(DataWin);
}

void UI::DrawControlWin() {
	// Clearing the window
	wclear(ControlWin);

	// Drawing the window's border
	box(ControlWin, 0, 0);
	mvwprintw(ControlWin, 0, 1, "Controls");

	// Drawing the window's contents
	mvwprintw(ControlWin, 1, 1, "Arrow keys to move");
	mvwprintw(ControlWin, 2, 1, "Z to undo");
	mvwprintw(ControlWin, 3, 1, "Q to quit");

	// Refreshing the window
	wrefresh(ControlWin);
}
