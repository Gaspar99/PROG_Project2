#ifndef PROG_PROJECT2_PUZZLE_H
#define PROG_PROJECT2_PUZZLE_H

#include <iostream>
#include "Board.h"
#include "Dictionary.h"
#include "Player.h"

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

	std::map<std::string, std::string> insertedWords;
	std::map<std::string, std::string> boardWords;

	void loadBoard();

    void handleAddWord();
	void insertWord(std::string word, char verCoord, char horCoord, char direction);

	bool boardIsFull();
    void handleReset();
    bool parseCoordinates(char verCoord, char horCoord, char direction);
	void showInstructions();
	void checkInsertedWords();
	std::string finalChecking();

    int fits(std::string word, std::string coord);
    bool matches(std::string word, std::string line);
	bool equalMaps();
};


#endif //PROG_PROJECT2_PUZZLE_H
