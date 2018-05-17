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
    void modifyMap(string word, coord initialCoord, char direction, int mode = 0, multimap<string, string> currentWords = {});
	void addWord(string word, coord initialCoord, char direction);
	void removeWord(coord initialCoord, char direction, multimap<string, string> currentWords);

	unsigned int getNumOfCols();
	unsigned int getNumOfRows();
	string getLine(char verCoord, char horCoord, char direction);

	bool nextCoordinates(char &verCoord, char &horCoord);

    bool isNotFull();

    void setWriteMode(int mode);

    void finish();
    char getValueAt(coord coordinate);
private:
	// The board itself is a map with strings as keys representing coordinates followed by a char value representing the
	// value of the corresponding cell.
	map<coord, char> board;
	size_t nRows; // number of rows in the board
	size_t nCols; // number of columns in the board
	int writeMode; // defines whether the board is to be written to a file (0) or to the screen (1)

    // Returns a pair of vectors. The first vector contains the coordinates of the cells to be modified.
    // The second one contains the coordinates of the previous cell and of the cell that comes after the word.
    // If any of these coordinates is out of bounds, the corresponding position in the vector will be empty.
    // Both vectors are initialized with null characters. Built into addWords.
    pair<vector<coord>, vector<coord>> generateCoordinates(unsigned int length, coord initialCoord, char direction);

    bool isCurrentWord(string word, multimap<string, string> currentWords);
    string getWord(coord initialCoordinate, char direction);
};

#endif //PROG_PROJECT2_BOARD_H
