# SimpleSoko
A basic terminal-based Sokoban engine written in C++, using the ncurses library.

## Usage
1. Install the ncurses development library
2. Clone the git repository and navigate into its directory
```console
git clone https://github.com/JosCla/SimpleSoko && cd SimpleSoko
```
3. Build the program
```console
make
```
4. Run the program
```console
./soks [path to level file]
```

## How to play
The goal of Sokoban is to push all of the boxes in a level onto the level's target squares. The boxes can only be pushed if there is no wall in their path, and two boxes in a row cannot be pushed at the same time.

Controls:
```
Arrow keys - Move
Z - Undo previous move
Q - Quit game
```

## Level files
### File format
The first three numbers in the file represent the height of the level, the width of the level, and the number of metadata entries to read.

The next few rows (specified by the height) represent the level itself. Here is what each character represents:
```
[#] - Wall
[ ] - Floor
[.] - Target
[@] - Player
[+] - Player on Target
[$] - Box
[*] - Box on Target
```

The final few rows (specified by the number of metadata entries) represent the metadata, with the text before the semicolon being the name of the entry, and the text after the semicolon being the value of the entry.

A sample level file (found at `levels/sample.soks`) follows:
```
4 6 3
######
#@ $.#
#   *#
######
Title:Sample
Author:J
Description:Sample level to demonstrate the file format
```

### Using the level files
In order to play a level, find the path of the level relative to the root directory of the program.

Then, use the following to play the level:
```console
./soks [path to level file]
```

For example, if your level file was located at `levels/sample.soks`, you would use the following:
```console
./soks levels/sample.soks
```

A few sample levels have been provided with the program, and they can be found in the `levels` directory. Take a look in there if you want to try some of them out!
