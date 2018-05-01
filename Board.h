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
	friend ostream& operator<<(ostream &out, Board &board);
	void reset();

	// The following function adds a word to the board if mode == 0 and removes it if mode == 1
	int addWord(string word, coord initialCoord, char direction, int mode);
	int removeWord(string word, coord initialCoord, char direction);

	void showMap();

	unsigned int getRows() const;
	unsigned int getColumns() const;

	string row(char verCoord, char horCoord);
	string column(char verCoord, char horCoord);

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
						//pair<vector<coord>, vector<coord>> generateCoords(unsigned int length, pair<char, char> initialCoord, char direction);

						// Template function to get the keys from a map, returns a vector of the type of the keys.
						// May not be needed.
	template <class T, class U>
	vector<T> getKeys(map<T, U> mapObject);
};

#endif //PROG_PROJECT2_BOARD_H
