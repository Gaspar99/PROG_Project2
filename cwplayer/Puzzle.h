#ifndef PROG_PROJECT2_PUZZLE_H
#define PROG_PROJECT2_PUZZLE_H

#include <iostream>
#include "Board.h"
#include "Dictionary.h"
#include "Player.h"

using namespace std;

class Puzzle
{
public:
    Puzzle();

    void greetUser();
    void showMenu();

private:
    Board board;
    Dictionary dictionary;
	Player player;
    string dictionaryFile;
	map<string, string> insertedWords;

	void loadBoard();

    void handleAddWord();
	void insertWord(string word, char verCoord, char horCoord, char direction);

	bool boardIsFull();
    void handleReset();
    bool parseCoordinates(char xCoord, char yCoord, char direction);
	void showInstructions();

    bool fits(string word, char verCoord, char horC, char direction);
    bool matches(string word, string line);
};


#endif //PROG_PROJECT2_PUZZLE_H
