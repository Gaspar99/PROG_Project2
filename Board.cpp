#include <iostream>
#include <iomanip>
#include "Board.h"
#include "colorizer.h"

using namespace std;

Board::Board(unsigned int nRows, unsigned int nColumns)
{
    // Initializes all fields of the board map with all possible Coordsinates and dots.
    // The board map itself doesn't have any letters until a word is added.
    for (int i = 0; i < nRows; ++i) {
        string line = string(1, char(i + 65));

        for (int j = 0; j < nColumns; ++j) {
            // builds a string like "Aa", "Ab", etc
            board.insert(pair<coord, char>(coord(static_cast<const char &>(i + 65),
                                                 static_cast<const char &>(j + 97)), '.'));
        }
    }

    this->nRows = nRows;
    this->nCols = nColumns;
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
        cout << setw(2) << left << string(1, char(i + 65)) << right << setw(2); // Prints the firstCoord letter.
        coord firstCoord(i + 65, 'a'); // Builds a firstCoord representing the first Coordinate of firstCoord i

        vector<coord> coordsInLine = generateCoords(nCols, firstCoord, 'H').first;

        for (coord c : coordsInLine) {
            if (board[c] == '#' && c != coordsInLine[0]) {
                setcolor(0);
                cout << " ";

                setcolor(0, 15);
                cout << '#';
            }
            else if (board[c] == '#') {
                cout << setw(1) << " ";
                setcolor(0, 15);
                cout << '#';
            }
            else {
                setcolor(15);
                cout << setw(2) << board[c];
            }
        }

        cout << endl;
    }
}

int Board::addWord(string word, coord initialCoord, char direction)
{
    pair<vector<coord>, vector<coord>> coordsToModify = generateCoords(word.length(), initialCoord, direction);

    // Checks if all Coordinates that are going to be modified are empty. If one isn't, return -1
    // TODO: This behaviour is for testing purposes only, it must be changed to check if a letter matches with an existing one
    //for (const string &coord : coordsToModify) {
    //    if (board[coord] != '.') return -1;
    //}

    int i = 0;
    for (coord c : coordsToModify.first) {
        board[c] = word[i];
        ++i;
    }

    for (coord coords : coordsToModify.second) {
        board[coords] = '#';
    }

    // Return 0 means the word was successfully added.
    return 0;
}

pair<vector<Board::coord>, vector<Board::coord>>
Board::generateCoords(unsigned int length, coord initialCoords, char direction)
{
    pair<vector<coord>, vector<coord>> coordsToModify;

    switch (direction) {
    case 'V': {
        auto newCoord = static_cast<char>(initialCoords.first + length - 1);

        if (newCoord < (nRows + 65)) {
            coord coord(++newCoord, initialCoords.second);
            coordsToModify.second.push_back(coord);
        }

        if (initialCoords.first > 'A') {
            coord coord(--initialCoords.first, initialCoords.second);
            coordsToModify.second.push_back(coord);
        }

        break;
    }
    case 'H': {
        auto newCoord = static_cast<char>(initialCoords.second + length - 1);

        if (newCoord < (nCols + 97)) {
            coord coord(initialCoords.first, newCoord);
            coordsToModify.second.push_back(coord);
        }

        if (initialCoords.second > 'a') {
            coord coord(initialCoords.first, --initialCoords.second);
            coordsToModify.second.push_back(coord);
        }

        break;
    }

    default:break;
    }

    switch (direction) {
    case 'V': {
        int j = 0;
        for (char i = initialCoords.first; j < length; ++i, ++j) {
            coord coord(i, initialCoords.second);

            coordsToModify.first.push_back(coord);
        }
        break;
    }

    case 'H': {
        int j = 0;
        for (char i = initialCoords.second; j < length; ++i, ++j) {
            coord coord(initialCoords.first, i);

            coordsToModify.first.push_back(coord);
        }
        break;
    }

    default:break;
    }

    return coordsToModify;
}

unsigned int Board::getRows() const
{
    return nRows;
}

unsigned int Board::getColumns() const
{
    return nCols;
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
