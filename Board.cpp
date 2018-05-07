#include <iostream>
#include <iomanip>
#include "Board.h"
#include "utils.h"

using namespace std;

Board::Board(unsigned int nRows, unsigned int nColumns)
{
    this->nRows = nRows;
    this->nCols = nColumns;

    // Initializes all fields of the board map with all possible Coordinates and dots.
    // The board map itself doesn't have any letters until a word is added.
    for (int i = 0; i < nRows; ++i) {
        for (int j = 0; j < nCols; ++j) {
            // builds a string like "Aa", "Ab", etc
            board.insert(pair<coord, char>(coord(static_cast<const char &>(i + 65),
                                                 static_cast<const char &>(j + 65)), '.'));
        }
    }
}

void Board::reset()
{
    for (int i = 0; i < nRows; ++i) {
        for (int j = 0; j < nCols; ++j) {
            board[(coord(static_cast<const char &>(i + 65), static_cast<const char &>(j + 65)))] = '.';
        }
    }
}

ostream &operator<<(ostream &out, Board &board)
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
            Board::coord c(char(i + 65), char(j + 65));

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

int Board::modifyMap(string word, coord initialCoord, char direction, int mode)
{
    size_t wordLength = word.length();
    pair<vector<coord>, vector<coord>> coordsToModify = generateCoords(wordLength, initialCoord, direction);

    for (int i = 0; i < wordLength; ++i) {
        board[coordsToModify.first[i]] = word[i];
    }

    for (int i = 0; i < wordLength; ++i) {
        board[coordsToModify.second[i]] = '#';
    }

    // Return 0 means the word was successfully added.
    return 0;
}

int Board::removeWord(string word, coord initialCoord, char direction)
{
    return modifyMap(word, initialCoord, direction);
}

int Board::addWord(string word, Board::coord initialCoord, char direction)
{
    return modifyMap(word, initialCoord, direction, 0);
}

pair<vector<Board::coord>, vector<Board::coord>>
Board::generateCoords(unsigned int wordLength, coord initialCoord, char direction)
{
    vector<coord> wordCoords;
    vector<coord> limitCoords;

    switch (direction) {
    case 'V': {
        int j = 0;
        for (char i = initialCoord.first; j < wordLength; ++i, ++j) {
            coord coord(i, initialCoord.second);
            wordCoords.push_back(coord);
        }
        break;
    }

    case 'H': {
        int j = 0;
        for (char i = initialCoord.second; j < wordLength; ++i, ++j) {
            coord coord(initialCoord.first, i);
            wordCoords.push_back(coord);
        }
        break;
    }

    default:break;
    }

    switch (direction) {
    case 'V': {
        auto newCoord = static_cast<char>(initialCoord.first + wordLength - 1);

        if (newCoord < (nRows + 65)) {
            coord coord(++newCoord, initialCoord.second);
            limitCoords.push_back(coord);
        }

        if (initialCoord.first > 'A') {
            coord coord(--initialCoord.first, initialCoord.second);
            limitCoords.push_back(coord);
        }
        break;
    }
    case 'H': {
        auto newCoord = static_cast<char>(initialCoord.second + wordLength - 1);

        if (newCoord < (nCols + 97)) {
            coord coord(initialCoord.first, ++newCoord);
            limitCoords.push_back(coord);
        }

        if (initialCoord.second > 'a') {
            coord coord(initialCoord.first, --initialCoord.second);
            limitCoords.push_back(coord);
        }
        break;
    }

    default:break;
    }

    return pair<vector<coord>, vector<coord>>(wordCoords, limitCoords);
}

bool Board::isNotSurrounded(coord coordinate, char charToCheck, char direction)
{
    switch (direction) {
    case 'V': {
        char left = --coordinate.second;
        char right = ++coordinate.second;

        return (left != '.' || right != '.');
    }

    case 'H': {
        char top = ++coordinate.first;
        char bottom = --coordinate.first;

        return (top != '.' || bottom != '.');
    }
    default: break;
    }

    return false;
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

unsigned int Board::getNumOfRows()
{
	return nRows;
}
unsigned int Board::getNumOfCols()
{
	return nCols;
}
//Returns the line (row or column, depending on 'direction') that follows the coordinates (verCoord, horCoord)
//(verCoord, horCoord - vertical coordinate, horizontal coordinate)
string Board::getLine(char verCoord, char horCoord, char direction)
{
	string line = "";
	coord coord(verCoord, horCoord);
	int i, max;

	if (direction == 'H') {
		i = (int)horCoord;

		for (i; i < nCols + 97; i++) {
			line = line + board.find(coord)->second;
			coord.second++;
		}
	}
	else {
		i = (int)verCoord;

		for (i; i < nRows + 65; i++) {
			line = line + board.find(coord)->second;
			coord.first++;
		}
	}

	return line;
}
//Receives a par of coordinates and changes them to the next coordinates.
//If the pair of coordinates received is the last one of the board, it returns false. Otherwise, returns true
bool Board::nextCoordinates(char &verCoord, char &horCoord)
{
    char lastVerCoord = 65 + nRows - 1;
    char lastHorCoord = 97 + nCols - 1;
    if (horCoord == lastHorCoord) {
        if (verCoord == lastVerCoord) { return false; }
        else {
            verCoord++;
            horCoord = 'a';
            return true;
        }
    }
    else {
        horCoord++;
        return true;
    }
}