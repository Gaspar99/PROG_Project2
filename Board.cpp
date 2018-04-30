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

        vector<coord> coordsInLine = generateCoords(nCols, firstCoord, 'H');

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
    vector<coord> coordsToModify = generateCoords(word.length(), initialCoord, direction);

    // These variables represent the Coordsinates that will be modified with a #
    coord previousCoord = generateCoords(-1, initialCoord, direction);
    coord lastCoord = generateCoords(-2, coordsToModify.back(), direction);

    // Checks if all Coordsinates that are going to be modified are empty. If one isn't, return -1
    // TODO: This behaviour is for testing purposes only, it must be changed to check if a letter matches with an existing one
    //for (const string &coord : coordsToModify) {
    //    if (board[coord] != '.') return -1;
    //}

    int i = 0;
    for (coord Coords : coordsToModify) {
        board[Coords] = word[i];
        ++i;
    }

    coordsToModify.clear();

    // Modify map if string returned from generateCoordss is valid (not empty)
    if (previousCoord.first != '\0') {
        coordsToModify.push_back(previousCoord);
    }

    if (lastCoord.first != '\0') {
        coordsToModify.push_back(lastCoord);
    }

    for (coord Coords : coordsToModify) {
        board[Coords] = '#';
        ++i;
    }

    // Return 0 means the word was successfully added.
    return 0;
}

vector<Board::coord> Board::generateCoords(unsigned int length, coord initialCoords, char direction)
{
    vector<coord> CoordsVector;

    switch (direction) {
    case 'V': {
        int j = 0;
        for (char i = initialCoords.first; j < length; ++i, ++j) {
            coord Coords(i, initialCoords.second);

            CoordsVector.push_back(Coords);
        }
        break;
    }

    case 'H': {
        int j = 0;
        for (char i = initialCoords.second; j < length; ++i, ++j) {
            coord line(initialCoords.first, i);

            CoordsVector.push_back(line);
        }
        break;
    }

    default:break;
    }

    return CoordsVector;
}

Board::coord Board::generateCoords(int length, coord initialCoords, char direction)
{
    coord coord('\0', '\0');

    if (length == -2) {
        switch (direction) {
        case 'V': {
            if (initialCoords.first < (nRows + 65)) {
                coord.first = ++initialCoords.first;
                coord.second = initialCoords.second;
            }

            break;
        }
        case 'H': {
            if (initialCoords.second < (nCols + 97)) {
                coord.first = initialCoords.first;
                coord.second = ++initialCoords.second;
            }
            break;
        }

        default:break;
        }
    }
    else if (length == -1) {
        switch (direction) {
        case 'V': {
            if (initialCoords.first > 'A') {
                coord.first = --initialCoords.first;
                coord.second = initialCoords.second;
            }

            break;
        }
        case 'H': {
            if (initialCoords.second > 'a') {
                coord.first = initialCoords.first;
                coord.second = --initialCoords.second;
            }
            break;
        }

        default:break;
        }
    }

    return coord;
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
