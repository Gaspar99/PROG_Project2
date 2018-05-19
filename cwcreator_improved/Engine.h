#ifndef PROG_PROJECT2_ENGINE_H
#define PROG_PROJECT2_ENGINE_H

#include "Logger.h"
#include "Dictionary.h"
#include "Board.h"

// Class Engine is responsible for controlling the interactions between the user and the program
// by showing menus and calling functions from its parent classes after processing user input if required.
// Such processing involves checks for length of the word, COORDINATE validity and word matching.
// Function names are similar to those found on Dictionary and Board classes to express conceptual proximity with them.
// The program is case-insensitive.
class Engine: public Dictionary, public Board
{
public:
    Engine();

    void start();
    void showMenu();

private: // Engine is not supposed to be some class's parent
    Logger logger;
    Utility utility;
    std::string dictionaryFile;

    // Game creation
    void createPuzzle();

    // Board modification (performs checks before calling Board member functions)
    void addWordDialogue();
    void insertWordDialogue(std::string word, char verCoord, char horCoord, char direction);
    void resetAllDialogue(); // Also modifies parameters from the Dictionary class

    // Features
    void suggestWordsDialogue(char verCoord, char horCoord, char direction);
    void suggestAllWordsDialogue();
    void checkAutomaticWordsDialogue(char line, char column, char direction);

    // Checks
    bool coordinateInBoard(COORDINATE coordinate);
    bool fits(std::string &word, char verCoord, char horCoord, char direction);
    bool matches(std::string word, std::string line);

    // Prepares write to file
    void writeDialogue();
};


#endif //PROG_PROJECT2_ENGINE_H
