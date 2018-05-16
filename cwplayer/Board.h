#ifndef PROG_PROJECT2_BOARD_H
#define PROG_PROJECT2_BOARD_H

#include <map>
#include <string>
#include <vector>

using namespace std;

class Board
{
    // Shows the board on the screen.
    friend ostream& operator<<(ostream &out, Board &board);

public:
	typedef pair<char, char> coord;

	// Constructor for the Board class which generates a map and its respective keys which represent coordinates on the board
	// Default values for each coordinate is a . character
	Board(unsigned int nRows, unsigned int nColumns);

	void reset();

	// The following function adds a word to the board if mode == 0 and removes it if mode == 1
    bool modifyMap(string word, coord initialCoord, char direction, int mode = 0);
	int addWord(string word, coord initialCoord, char direction);
	bool removeWord(coord initialCoord, char direction, string insertedWord);

	void insertWhiteCells(string word, coord initialCoord, char);

	unsigned int getNumOfCols();
	unsigned int getNumOfRows();

	string getLine(char verCoord, char horCoord, char direction);

private:
	// The board itself is a map with strings as keys representing coordinates followed by a char value representing the
	// value of the corresponding cell.
	map<coord, char> board;
	unsigned int nRows; // number of rows in the board
	unsigned int nCols; // number of columns in the board

    // Returns a pair of vectors. The first vector contains the coordinates of the cells to be modified.
    // The second one contains the coordinates of the previous cell and of the cell that comes after the word.
    // If any of these coordinates is out of bounds, the corresponding position in the vector will be empty.
    // Both vectors are initialized with null characters. Built into addWords.
    vector<coord> generateCoords(unsigned int length, pair<char, char> initialCoord, char direction);

    bool isNotSurrounded(coord coordinate, char direction);
};

#endif //PROG_PROJECT2_BOARD_H
