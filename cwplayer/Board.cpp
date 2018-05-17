#include <iostream>
#include <iomanip>
#include "Board.h"
#include "utils.h"
#include "Player.h"

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
				static_cast<const char &>(j + 65)), '#'));
		}
	}
}

void Board::reset()
{
	for (int i = 0; i < nRows; ++i) {
		for (int j = 0; j < nCols; ++j) {
			coord coord(static_cast<const char &>(i + 65), static_cast<const char &>(j + 65));

			if (board[coord] != '.')
				board[coord] = '#';
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

bool Board::modifyMap(string word, coord initialCoord, char direction, int mode)
{
	size_t wordLength = word.length();
	vector<coord> coordsToModify = generateCoords(wordLength, initialCoord, direction);

	for (int i = 0; i < wordLength; ++i) {
		coord currentCoord = coordsToModify[i];
		char& currentLetter = board[currentCoord];

		if ((!isalpha(currentLetter) && mode == 0) || isNotSurrounded(currentCoord, direction) && mode == 1)
			currentLetter = word[i];

	}

	// Return true means the word was successfully added.
	return true;
}

bool Board::removeWord(coord initialCoord, char direction, string insertedWord)
{
	string coord, word;

	word = string(insertedWord.length(), '.');

	return modifyMap(word, initialCoord, direction, 1);

}

unsigned int Board::getNumOfCols()
{
	return nCols;
}

unsigned int Board::getNumOfRows()
{
	return nRows;
}

int Board::addWord(string word, Board::coord initialCoord, char direction)
{
	return modifyMap(word, initialCoord, direction);
}

vector<Board::coord> Board::generateCoords(unsigned int wordLength, coord initialCoord, char direction)
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

	return wordCoords;
}

//Returns the line (row or column, depending on 'direction') that follows the coordinates (verCoord, horCoord)
//(verCoord, horCoord - vertical coordinate, horizontal coordinate)
string Board::getLine(char verCoord, char horCoord, char direction)
{
	string line;
	coord coordinate(verCoord, horCoord);
	vector<coord> coordsToModify;

	switch (direction) {
	case 'H':
		coordsToModify = generateCoords(nCols - horCoord + 65, coordinate, 'H');

		for (coord c : coordsToModify) {
			line.push_back(board[c]);
		}

		return line;
	case 'V':
		coordsToModify = generateCoords(nRows - verCoord + 65, coordinate, 'V');

		for (coord c : coordsToModify) {
			line.push_back(board[c]);
		}

		return line;

	default:break;
	}

	return line;
}

void Board::insertWhiteCells(string word, coord initialCoord, char direction)
{
	vector<coord> coordsToModify = generateCoords(word.length(), initialCoord, direction);

	for (const auto &i : coordsToModify) {
		board[i] = '.';
	}

}

bool Board::isNotSurrounded(coord coordinate, char direction)
{
	switch (direction) {
	case 'V': {
		coord left(coordinate.first, static_cast<const char &>(coordinate.second - 1));
		coord right(coordinate.first, static_cast<const char &>(coordinate.second + 1));

		return (!isalpha(board[left]) && !isalpha(board[right]));
		//return board[left] == '.' && (board[right] == '.' || board[right] == '\0') || (board[right] == '.' && board[left] == '\0');
	}

	case 'H': {
		coord top(static_cast<const char &>(coordinate.first + 1), coordinate.second);
		coord bottom(static_cast<const char &>(coordinate.first - 1), coordinate.second);

		return (!isalpha(board[top]) && !isalpha(board[bottom]));
		//return board[top] == '.' && (board[bottom] == '.' || board[bottom] == '\0') || (board[bottom] == '.' && board[top] == '\0');
	}
	default: break;
	}

	return false;
}
