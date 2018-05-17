#include <iostream>
#include <iomanip>
#include <algorithm>
#include "Board.h"
#include "utils.h"
#include "Dictionary.h"

using namespace std;

Board::Board(unsigned int nRows, unsigned int nColumns)
{
    this->nRows = nRows;
    this->nCols = nColumns;

    // Initializes all fields of the board map with all possible Coordinates and dots.
    // The board map itself doesn't have any letters until a word is added.
    for (size_t i = 0; i < nRows; ++i) {
        for (size_t j = 0; j < nCols; ++j) {
            // builds a string like "Aa", "Ab", etc
            board.insert(pair<coord, char>(coord(static_cast<const char &>(i + 65),
                                                 static_cast<const char &>(j + 65)), '.'));
        }
    }
}

void Board::reset()
{
    for (size_t i = 0; i < nRows; ++i) {
        for (size_t j = 0; j < nCols; ++j) {
            board[(coord(static_cast<const char &>(i + 65), static_cast<const char &>(j + 65)))] = '.';
        }
    }
}

ostream &operator<<(ostream &out, Board &board)
{
    // Prints the first line of the board, which corresponds to the letters of the columns.
    if (board.writeMode) {
        for (size_t i = 0; i < board.nCols; ++i) {
            if (i == 0) {
                setcolor(3);
                out << setw(4) << right << string(1, char(i + 97));
            }
            else {
                out << setw(2) << string(1, char(i + 97));
            }
        }
    }

    out << endl;

    // Prints the remainder of the lines.
    for (size_t i = 0; i < board.nRows; ++i) {
        if (board.writeMode) {
            setcolor(3);
            out << setw(2) << left << string(1, char(i + 65)) << right << setw(1); // Prints the firstCoord letter.
        }

        for (size_t j = 0; j < board.nCols; ++j) {
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

bool Board::modifyMap(string word, coord initialCoord, char direction, int mode, map<string, string> currentWords)
{
    size_t wordLength = word.length();
    pair<vector<coord>, vector<coord>> coordsToModify = generateCoordinates(wordLength, initialCoord, direction);

    if (direction == 'H') { direction = 'V'; } else if (direction == 'V') { direction = 'H'; };

    for (size_t i = 0; i < wordLength; ++i) {
        coord currentCoord = coordsToModify.first[i];
        char &currentLetter = board[currentCoord];

        string wordLine = getWord(currentCoord, direction);

        if ((!isalpha(currentLetter) && mode == 0) || (!isCurrentWord(wordLine, currentWords) && mode == 1))
            currentLetter = word[i];
    }

    for (const auto &it : coordsToModify.second) {
        char &currentLetter = board[it];

        if (!isalpha(currentLetter) && currentLetter != '#' && mode == 0)
            currentLetter = '#';
        else if (!isalpha(currentLetter) && mode == 1)
            currentLetter = '.';
    }

    // Return true means the word was successfully added.
    return true;
}

bool Board::removeWord(coord initialCoord, char direction, map<string, string> currentWords)
{
    switch (direction) {
    case 'H': {
        unsigned int i = 0;

        while (initialCoord.second + i <= (nCols + 65)) {
            coord next(initialCoord.first, static_cast<const char &>(initialCoord.second + i));
            if (board[next] == '#') break;
            ++i;
        }

        string word = string(i, '.');

        return modifyMap(word, initialCoord, direction, 1, currentWords);
    }

    case 'V': {
        unsigned int i = 0;

        while (initialCoord.first + i <= (nRows + 65)) {
            coord next(static_cast<const char &>(initialCoord.first + i), initialCoord.second);
            if (board[next] == '#') break;
            ++i;
        }

        string word = string(i, '.');

        return modifyMap(word, initialCoord, direction, 1, currentWords);
    }
    default:break;
    }
}

int Board::addWord(string word, Board::coord initialCoord, char direction)
{
    return modifyMap(word, initialCoord, direction, 0);
}

pair<vector<Board::coord>, vector<Board::coord>>
Board::generateCoordinates(unsigned int wordLength, coord initialCoord, char direction)
{
    vector<coord> wordCoords;
    vector<coord> limitCoords;

    switch (direction) {
    case 'V': {
        int j = 0;
        for (auto i = static_cast<unsigned char>(initialCoord.first); j < wordLength; ++i, ++j) {
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

        if (initialCoord.second > 'A') {
            coord coord(initialCoord.first, --initialCoord.second);
            limitCoords.push_back(coord);
        }
        break;
    }

    default:break;
    }

    return pair<vector<coord>, vector<coord>>(wordCoords, limitCoords);
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
    string line;
    coord coordinate(verCoord, horCoord);
    vector<coord> coordsToModify;

    switch (direction) {
    case 'H':coordsToModify = generateCoordinates(nCols - horCoord + 65, coordinate, 'H').first;

        for (coord c : coordsToModify) {
            line.push_back(board[c]);
        }

        return line;
    case 'V':coordsToModify = generateCoordinates(nRows - verCoord + 65, coordinate, 'V').first;

        for (coord c : coordsToModify) {
            line.push_back(board[c]);
        }

        return line;

    default:break;
    }

    return line;
}
//Receives a par of coordinates and changes them to the next coordinates.
//If the pair of coordinates received is the last one of the board, it returns false. Otherwise, returns true
bool Board::nextCoordinates(char &verCoord, char &horCoord)
{
    auto lastVerCoord = static_cast<char>(65 + nRows - 1);
    auto lastHorCoord = static_cast<char>(65 + nCols - 1);
    if (horCoord == lastHorCoord) {
        if (verCoord == lastVerCoord) { return false; }
        else {
            verCoord++;
            horCoord = 'A';
            return true;
        }
    }
    else {
        horCoord++;
        return true;
    }
}
bool Board::isNotFull()
{
    for (const auto &it : board)
        if (it.second == '.')
            return true;

    return false;
}

void Board::setWriteMode(int mode)
{
    writeMode = mode;
}

void Board::finish()
{
    for (auto &it : board)
        if (it.second == '.')
            it.second = '#';
}
char Board::getValueAt(Board::coord coordinate)
{
    return board[coordinate];
}

bool Board::isCurrentWord(string word, map<string, string> currentWords)
{
    for (const auto &it : currentWords) {
        if (word == it.second)
            return true;
    }

    return false;
}
string Board::getWord(coord initialCoordinate, char direction)
{
    string word;
    coord initial = initialCoordinate;

    switch (direction) {
    case 'V': {
        while (board[initial] != '#' && board[initial] != '\0') {
            word.push_back(board[initial]);
            --initial.first;
        }
        break;
    }
    case 'H': {
        while (board[initial] != '#' && board[initial] != '\0') {
            word.push_back(board[initial]);
            --initial.second;
        }
        break;
    }
    default: break;
    }

    reverse(word.begin(), word.end());

    switch (direction) {
    case 'V': {
        ++initialCoordinate.first;
        while (board[initialCoordinate] != '#' && board[initialCoordinate] != '\0') {
            word.push_back(board[initialCoordinate]);
            ++initialCoordinate.first;
        }
        break;
    }
    case 'H': {
        ++initialCoordinate.second;
        while (board[initialCoordinate] != '#' && board[initialCoordinate] != '\0') {
            word.push_back(board[initialCoordinate]);
            ++initialCoordinate.second;
        }
        break;
    }
    default: break;
    }

    return word;
}

