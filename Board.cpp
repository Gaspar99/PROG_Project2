#include <iostream>
#include <iomanip>
#include <cstdarg>
#include "Board.h"
#include "colorizer.h"

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

void Board::showBoard()
{
    // Prints the first line of the board, which corresponds to the letters of the columns.
    for (unsigned int i = 0; i < nCols; ++i) {
        if (i == 0) {
            setcolor(3);
            cout << setw(4) << right << string(1, char(i + 97));
        }
        else {
            cout << setw(2) << string(1, char(i + 97));
        }
    }

    cout << endl;

    // Prints the remainder of the lines.
    for (unsigned int i = 0; i < nRows; ++i) {
        setcolor(3);
        cout << setw(2) << left << string(1, char(i + 65)) << right << setw(1); // Prints the firstCoord letter.

        for (unsigned int j = 0; j < nCols; ++j) {
            coord c(char(i + 65), char(j + 97));

            if (board[c] == '#' && c != pair<char, char>(char(i), char(i + 32))) {
                cout << " ";
                setcolor(0, 15);
                cout << setw(1) << '#';
            }
            else if (board[c] == '#') {
                setcolor(0, 15);
                cout << left << setw(1) << '#' << right;
            }
            else {
                setcolor(15);
                cout << setw(2) << board[c];
            }
        }

        cout << endl;
    }
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