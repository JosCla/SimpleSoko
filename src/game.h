#ifndef GAME_H
#define GAME_H
// game.h
// Contains game-related classes

// C++ Standard Library
#include <vector>
#include <string>

// Libraries
#include <ncurses.h>

struct Player {
	int posX, posY;
};

struct Box {
	int posX, posY;
};

struct Metadata {
	std::string name;
	std::string value;
};

enum Direction {
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT
};

class Game {
	public: 
		Game();
		Game(const char *filename);
		int LoadData(const char *filename);
		void ToggleColor();

		bool MovePlayer(Direction dir);
		bool MoveBox(unsigned int ind, Direction dir);
		int BoxPresent(int posX, int posY);
		void UndoMove();

		void DrawBoard(WINDOW *win);
		bool CheckWin();

		vector<Metadata> GetMetadata();

	private:
		std::vector< std::vector<char> > Level;
		std::vector<Metadata> LevelMetadata;

		unsigned int LevelWidth, LevelHeight;
		Player LevelPlayer;
		std::vector<Box> LevelBoxes;

		std::vector<char> LevelMoves;

		bool ColorOn;
};

#endif // GAME_H
