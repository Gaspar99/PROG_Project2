#ifndef PROG_PROJECT2_BOARD_H
#define PROG_PROJECT2_BOARD_H

#include <map>
#include <string>
#include <vector>

using namespace std;

class Board
{
public:
    typedef pair<char, char> coord;

    // Constructor for the Board class which generates a map and its respective keys which represent coordinates on the board
    // Default values for each coordinate is a . character
    Board(unsigned int nRows, unsigned int nColumns);

    // Shows the board on the screen. TODO: change function so that it can also write to a file by modifying the prototype
    void showBoard();
    int addWord(string word, pair<char, char> initialCoord, char direction);

    unsigned int getRows() const;
    unsigned int getColumns() const;

private:
    // The board itself is a map with strings as keys representing coordinates followed by a char value representing the
    // value of the corresponding cell.
    map<pair<char, char>, char> board;
    unsigned int nRows; // number of rows in the board
    unsigned int nCols; // number of columns in the board

    // Returns a single coordinate. Used for the very special cases where the previous and the next coordinates are needed,
    // taking into account the direction given by the user.
    coord generateCoords(int length, pair<char, char> initialCoord, char direction);

    // Returns a vector of coordinates corresponding to those which have to be filled in.
    vector<coord> generateCoords(unsigned int length, pair<char, char> initialCoord, char direction);

    // Template function to get the keys from a map, returns a vector of the type of the keys.
    // May not be needed.
    template <class T, class U>
    vector<T> getKeys(map<T, U> mapObject);
};


#endif //PROG_PROJECT2_BOARD_H
