#ifndef PROG_PROJECT2_BOARD_H
#define PROG_PROJECT2_BOARD_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "Utility.h"

namespace BoardTypes // defines a new type called COORDINATE to represent a COORDINATE
{
typedef std::pair<char, char> COORDINATE;
}
using namespace BoardTypes;

// Class Board represents the board where words are added or removed and implements functions for that
class Board
{
public:
    Board();

    // Generates a map and its respective keys which represent coordinates on the board
    // Default values for each COORDINATE is a . character
    void makeBoard(unsigned int nRows, unsigned int nCols);

    // Board modification
    void reset();
    void removeWord(COORDINATE initialCoord, char direction, const std::multimap<std::string, std::string> &currentWords = {});
    void addWord(const std::string &word, COORDINATE initialCoord, char direction);

    // Checks
    bool isNotFull();

    std::string getLine(char verCoord, char horCoord, char direction); // Returns the word in the perpendicular column or line
    bool nextCoordinates(char &verCoord, char &horCoord);
    std::ostream &writeBoard(std::ostream &out, int writeMode);
    void finish();

private:
    Utility utility;

    // Returns a pair of vectors. The first vector contains the coordinates of the cells to be modified.
    // The second one contains the coordinates of the previous cell and of the cell that comes after the word.
    // If any of these coordinates is out of bounds, the corresponding position in the vector will be empty.
    // Both vectors are initialized with null characters. Built into addWords.
    std::pair<std::vector<COORDINATE>, std::vector<COORDINATE>>
    generateCoordinates(unsigned int length, COORDINATE initialCoord, char direction);

    // Check if it is part of the currentWords map from
    bool isCurrent(std::string word, std::multimap<std::string, std::string> currentWords);
    std::string getWord(COORDINATE initialCoordinate, char direction);

    // The following function adds a word to the board if mode == 0 and removes it if mode == 1
    void modifyMap(std::string word,
                   COORDINATE initialCoord,
                   char direction,
                   int mode,
                   const std::multimap<std::string, std::string> &currentWords = {});

protected:
    // The board itself is a map with strings as keys representing coordinates followed by a char value representing the
    // value of the corresponding cell.
    std::map<COORDINATE, char> board;
    size_t nRows; // number of rows in the board
    size_t nCols; // number of columns in the board
};


#endif //PROG_PROJECT2_BOARD_H
