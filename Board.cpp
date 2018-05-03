#include <iostream>
#include <iomanip>
#include "Board.h"
#include "utils.h"

using namespace std;

Board::Board(unsigned int nRows, unsigned int nColumns)
{
    this->nRows = nRows;
    this->nCols = nColumns;

    // Initializes all fields of the board map with all possible Coordsinates and dots.
    // The board map itself doesn't have any letters until a word is added.
    for (int i = 0; i < nRows; ++i) {
        string line = string(1, char(i + 65));

        for (int j = 0; j < nCols; ++j) {
            // builds a string like "Aa", "Ab", etc
            board.insert(pair<coord, char>(coord(static_cast<const char &>(i + 65),
                                                 static_cast<const char &>(j + 97)), '.'));
        }
    }
}

void Board::reset()
{
    for (int i = 0; i < nRows; ++i) {
        string line = string(1, char(i + 65));

        for (int j = 0; j < nCols; ++j) {
            board[(coord(static_cast<const char &>(i + 65), static_cast<const char &>(j + 97)))] = '.';
        }
    }
}

ostream& operator<<(ostream &out, Board &board)
{
    // Prints the first line of the board, which corresponds to the letters of the columns.
    for (unsigned int i = 0; i < board.nCols; ++i) {
        if (i == 0) {
            setcolor(3);
            out << setw(4) << right << string(1, char(i + 97));
        }
        else {
            out << setw(2) << string(1, char(i + 97));
        }
    }

    out << endl;

    // Prints the remainder of the lines.
    for (unsigned int i = 0; i < board.nRows; ++i) {
        setcolor(3);
        out << setw(2) << left << string(1, char(i + 65)) << right << setw(1); // Prints the firstCoord letter.

        for (unsigned int j = 0; j < board.nCols; ++j) {
            Board::coord c(char(i + 65), char(j + 97));

            if (board.board[c] == '#' && c != pair<char, char>(char(i), char(i + 32))) {
                out << " ";
                setcolor(0, 15);
                out << setw(1) << '#';
            }
            else if (board.board[c] == '#') {
                setcolor(0, 15);
                out << left << setw(1) << '#' << right;
            }
            else {
                setcolor(15);
                out << setw(2) << board.board[c];
            }
        }

        out << endl;
    }

    return out;
}

int Board::addWord(string word, coord initialCoord, char direction, int mode)
{
    size_t length = word.length();

    switch (direction) {
    case 'V': {
        int j = 0;
        for (char i = initialCoord.first; j < length; ++i, ++j) {
            coord coord(i, initialCoord.second);
            mode ? board[coord] = '.' : board[coord] = word[j];
        }
        break;
    }

    case 'H': {
        int j = 0;
        for (char i = initialCoord.second; j < length; ++i, ++j) {
            coord coord(initialCoord.first, i);
            mode ? board[coord] = '.' : board[coord] = word[j];
        }
        break;
    }

    default:break;
    }

    switch (direction) {
    case 'V': {
        auto newCoord = static_cast<char>(initialCoord.first + length - 1);

        if (newCoord < (nRows + 65)) {
            coord coord(++newCoord, initialCoord.second);
            mode ? board[coord] = '.' : board[coord] = '#';
        }

        if (initialCoord.first > 'A') {
            coord coord(--initialCoord.first, initialCoord.second);
            mode ? board[coord] = '.' : board[coord] = '#';
        }

        break;
    }
    case 'H': {
        auto newCoord = static_cast<char>(initialCoord.second + length - 1);

        if (newCoord < (nCols + 97)) {
            coord coord(initialCoord.first, ++newCoord);
            mode ? board[coord] = '.' : board[coord] = '#';
        }

        if (initialCoord.second > 'a') {
            coord coord(initialCoord.first, --initialCoord.second);
            mode ? board[coord] = '.' : board[coord] = '#';
        }

        break;
    }

    default:break;
    }

    // Return 0 means the word was successfully added.
    return 0;
}

unsigned int Board::getRows() const
{
    return nRows;
}

unsigned int Board::getColumns() const
{
    return nCols;
}

int Board::removeWord(string word, coord initialCoord, char direction)
{
    return addWord(word, initialCoord, direction, 1);
}

//Potentially unnecessary
template<class T, class U>
vector<T> Board::getKeys(map<T, U> mapObject)
{
    vector<T> vector;

    for (const auto &it : mapObject) {
        vector.push_back(it.first);
    }

    return vector;
}

void Board::showMap()
{
	for (auto it = board.cbegin(); it != board.cend(); ++it) {
		cout << '(' << it->first.first << "," << it->first.second << ')' << " = " << it->second << endl;
	}
}

//Returns the row that follows the coordinates (verCoord, horCoord) - (vertical coordinate, horizontal coordinate)
string Board::row(char verCoord, char horCoord)
{
	string line = "";
	coord coord(verCoord, horCoord);
	int max = nCols - (horCoord - 97);
	for (int i = 0; i < max; i++) {
		line = line + board.find(coord)->second;
		coord.second = horCoord++;
	}

	return line;
}

//Returns the column that follows the coordinates (verCoord, horCoord) - (vertical coordinate, horizontal coordinate)
string Board::column(char verCoord, char horCoord)
{
	string col = "";
	coord coord(verCoord, horCoord);
	int max = nRows - (verCoord - 65);
	for (int i = 0; i < max; i++) {
		col = col + board.find(coord)->second;
		coord.first = verCoord++;
	}

	return col;
}