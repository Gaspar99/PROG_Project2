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
    map<string, string> currentWords;
    string dictionaryFile;

    void handleAddWord();
    void handleWrite();

    void handleSuggestWords();
    void handleReset();
    void parseCoordinates(char first, char second, char direction);
};


#endif //PROG_PROJECT2_PUZZLE_H
