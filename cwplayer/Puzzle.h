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
	map<string, string> boardWords;

	void loadBoard();

    void handleAddWord();
	void insertWord(string word, char verCoord, char horCoord, char direction);

	bool boardIsFull();
    void handleReset();
    bool parseCoordinates(char verCoord, char horCoord, char direction);
	void showInstructions();
	void checkInsertedWords();
	string finalChecking();

    int fits(string word, string coord);
    bool matches(string word, string line);
	bool equalMaps();
};


#endif //PROG_PROJECT2_PUZZLE_H
