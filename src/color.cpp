// color.cpp

// Libraries
#include <ncurses.h>

// Our files
#include "color.h"

void setupColor() {
	init_color(COLOR_BROWN, 568, 314, 176);

	init_pair(COLOR_BOX, COLOR_BLACK, COLOR_RED);
	init_pair(COLOR_PLAYER, COLOR_CYAN, COLOR_BLACK);
	init_pair(COLOR_BOX_TARGET, COLOR_BLACK, COLOR_GREEN);
	init_pair(COLOR_PLAYER_TARGET, COLOR_CYAN, COLOR_BLUE);
	init_pair(COLOR_TARGET, COLOR_BLACK, COLOR_BLUE);
	init_pair(COLOR_WALL, COLOR_BROWN, COLOR_BLACK);
	init_pair(COLOR_FLOOR, COLOR_WHITE, COLOR_BLACK);
}
