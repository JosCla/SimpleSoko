#ifndef UI_H
#define UI_H
// ui.h
// Handles the user interface

// C++ Standard Library
#include <vector>

// Libraries
#include <ncurses.h>

// Our files
#include "game.h"

class UI {
	public:
		UI();
		UI(WINDOW *gameWin, WINDOW *dataWin, WINDOW *controlWin);

		void AllocateWindows();

		void DrawGameWin(Game &game);
		void DrawDataWin(const std::vector<Metadata> &data);
		void DrawControlWin();

	private:
		WINDOW *GameWin;
		WINDOW *DataWin;
		WINDOW *ControlWin;
};

#endif // UI_H
