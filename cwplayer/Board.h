#ifndef PROG_PROJECT2_BOARD_H
#define PROG_PROJECT2_BOARD_H

#include <map>
#include <string>
#include <vector>

namespace BoardTypes
{
typedef std::pair<char, char> COORDINATE;
}

using namespace BoardTypes;

class Board
{
    // Shows the board on the screen.
    friend std::ostream &operator<<(std::ostream &out, Board &board);

public:
    // Constructor for the Board class which generates a map and its respective keys which represent coordinates on the board
    // Default values for each coordinate is a . character
    Board(unsigned int nRows, unsigned int nColumns);

    void reset(); //Replaces every char with a dot

    // The following function adds a word to the board if mode == 0 and removes it if mode == 1
    void modifyMap(std::string word, COORDINATE initialCoord, char direction, int mode = 0);
    void addWord(std::string word, COORDINATE initialCoord, char direction);
    void removeWord(COORDINATE initialCoord, char direction);

    //Inserts dots on the positions occupied by words
    void insertWhiteCells(std::string word, COORDINATE initialCoord, char);

    std::string getLine(char verCoord, char horCoord, char direction);

private:
    // The board itself is a map with strings as keys representing coordinates followed by a char value representing the
    // value of the corresponding cell.
    std::map<COORDINATE, char> board;
    unsigned int nRows; // number of rows in the board
    unsigned int nCols; // number of columns in the board

    // Returns a vector of strings containing the coordinates of the cells to be modified.
    // If any of these coordinates is out of bounds, the corresponding position in the vector will be empty.
    // The vector is initialized with null characters. Built into addWords.
    std::vector<COORDINATE> generateCoords(unsigned int length, COORDINATE initialCoord, char direction);

    std::string getWord(COORDINATE initialCoordinate, char direction);
};

#endif //PROG_PROJECT2_BOARD_H
