#ifndef PROG_PROJECT2_PUZZLE_H
#define PROG_PROJECT2_PUZZLE_H

#include <iostream>
#include "Board.h"
#include "Dictionary.h"

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
    string dictionaryFile;

	void createPuzzle();
	void loadPuzzle();

    void handleAddWord();
    void handleWrite();
	void insertWord(string word, char verCoord, char horCoord, char direction);

	void handleSuggestWords(char verCoord, char horCoord, char direction);
    void handleSuggestAllWords();
    void handleReset();
    bool parseCoordinates(char xCoord, char yCoord, char direction);
	void showInstructions();

    bool fits(string word, char verCoord, char horC, char direction);
    bool matches(string word, string line);
};


#endif //PROG_PROJECT2_PUZZLE_H
