#include "Board.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <utility>
#include "Board.h"
#include "Dictionary.h"

using namespace std;

Board::Board() = default;

void Board::makeBoard(unsigned int nRows, unsigned int nColumns)
{
    this->nRows = nRows;
    this->nCols = nColumns;

    // Initializes all fields of the board map with all possible Coordinates and dots.
    // The board map itself doesn't have any letters until a word is added.
    for (size_t i = 0; i < nRows; ++i) {
        for (size_t j = 0; j < nCols; ++j) {
            // builds a string like "Aa", "Ab", etc
            board.insert(pair<COORDINATE, char>(COORDINATE(static_cast<const char &>(i + 65),
                                                 static_cast<const char &>(j + 65)), '.'));
        }
    }
}

// Resets the board to its original state
void Board::reset()
{
    for (size_t i = 0; i < nRows; ++i) {
        for (size_t j = 0; j < nCols; ++j) {
            board[(COORDINATE(static_cast<const char &>(i + 65), static_cast<const char &>(j + 65)))] = '.';
        }
    }
}

// Writes to a file in the proper format if writeMode == 0 or to the screen if writeMode == 1
ostream &Board::writeBoard(ostream &out, int writeMode)
{
    // Prints the first line of the board, which corresponds to the letters of the columns.
    if (writeMode) {
        for (size_t i = 0; i < nCols; ++i) {
            if (i == 0) {
                utility.setcolor(CYAN);
                out << setw(4) << right << string(1, char(i + 97));
            }
            else {
                out << setw(2) << string(1, char(i + 97));
            }
        }
    }

    out << endl;

    // Prints the remainder of the lines.
    for (size_t i = 0; i < nRows; ++i) {
        if (writeMode) {
            utility.setcolor(CYAN);
            out << setw(2) << left << string(1, char(i + 65)) << right << setw(1); // Prints the firstCoord letter.
        }

        for (size_t j = 0; j < nCols; ++j) {
            COORDINATE c(char(i + 65), char(j + 65));

            if (board[c] == '#' && c != pair<char, char>(char(i), char(i + 32))) {
                out << " ";

                #if defined(_WIN32)
                    utility.setcolor(BLACK, WHITE);
                #elif defined(__unix__) || defined(__linux__) || defined(__APPLE__)
                    utility.setcolor(BLACK, WHITE_B);
                #endif

                out << setw(1) << '#';
            }
            else if (board[c] == '#') {
                #if defined(_WIN32)
                    utility.setcolor(BLACK, WHITE);
                #elif defined(__unix__) || defined(__linux__) || defined(__APPLE__)
                    utility.setcolor(BLACK, WHITE_B);
                #endif

                out << left << setw(1) << '#' << right;
            }
            else {
                utility.setcolor(WHITE);
                out << setw(2) << board[c];
            }
        }

        out << endl;
    }

    return out;
}

void Board::modifyMap(string word, COORDINATE initialCoord, char direction, int mode, const multimap<string, string> &currentWords)
{
    size_t wordLength = word.length();
    pair<vector<COORDINATE>, vector<COORDINATE>> coordsToModify = generateCoordinates(wordLength, initialCoord, direction);

    if (direction == 'H') { direction = 'V'; } else if (direction == 'V') { direction = 'H'; };

    for (size_t i = 0; i < wordLength; ++i) {
        COORDINATE currentCoord = coordsToModify.first[i];
        char &currentLetter = board[currentCoord];

        string wordLine = getWord(currentCoord, direction);

        if ((!isalpha(currentLetter) && mode == 0) || (!isCurrent(wordLine, currentWords) && mode == 1))
            currentLetter = word[i];
    }

    for (const auto &it : coordsToModify.second) {
        char &currentLetter = board[it];

        if (!isalpha(currentLetter) && currentLetter != '#' && mode == 0)
            currentLetter = '#';
        else if (!isalpha(currentLetter) && mode == 1)
            currentLetter = '.';
    }
}

void Board::removeWord(COORDINATE initialCoord, char direction, const multimap<string, string> &currentWords)
{
    switch (direction) {
    case 'H': {
        unsigned int i = 0;

        while (initialCoord.second + i <= (nCols + 65)) {
            COORDINATE next(initialCoord.first, static_cast<const char &>(initialCoord.second + i));
            if (board[next] == '#') break;
            ++i;
        }

        string word = string(i, '.');

        return modifyMap(word, initialCoord, direction, 1, currentWords);
    }

    case 'V': {
        unsigned int i = 0;

        while (initialCoord.first + i <= (nRows + 65)) {
            COORDINATE next(static_cast<const char &>(initialCoord.first + i), initialCoord.second);
            if (board[next] == '#') break;
            ++i;
        }

        string word = string(i, '.');

        return modifyMap(word, initialCoord, direction, 1, currentWords);
    }
    default:break;
    }
}

void Board::addWord(const string &word, COORDINATE initialCoord, char direction)
{
    return modifyMap(word, initialCoord, direction, 0);
}

pair<vector<COORDINATE>, vector<COORDINATE>>
Board::generateCoordinates(unsigned int wordLength, COORDINATE initialCoord, char direction)
{
    vector<COORDINATE> wordCoords;
    vector<COORDINATE> limitCoords;

    switch (direction) {
    case 'V': {
        size_t j = 0;
        for (auto i = static_cast<unsigned char>(initialCoord.first); j < wordLength; ++i, ++j) {
            COORDINATE coord(i, initialCoord.second);
            wordCoords.push_back(coord);
        }
        break;
    }

    case 'H': {
        size_t j = 0;
        for (char i = initialCoord.second; j < wordLength; ++i, ++j) {
            COORDINATE coord(initialCoord.first, i);
            wordCoords.push_back(coord);
        }
        break;
    }

    default:break;
    }

    switch (direction) {
    case 'V': {
        auto newCoord = static_cast<unsigned char>(initialCoord.first + wordLength - 1);

        if (newCoord < (nRows + 65)) {
            COORDINATE coord(++newCoord, initialCoord.second);
            limitCoords.push_back(coord);
        }

        if (initialCoord.first > 'A') {
            COORDINATE coord(--initialCoord.first, initialCoord.second);
            limitCoords.push_back(coord);
        }
        break;
    }
    case 'H': {
        auto newCoord = static_cast<unsigned char>(initialCoord.second + wordLength - 1);

        if (newCoord < (nCols + 97)) {
            COORDINATE coord(initialCoord.first, ++newCoord);
            limitCoords.push_back(coord);
        }

        if (initialCoord.second > 'A') {
            COORDINATE coord(initialCoord.first, --initialCoord.second);
            limitCoords.push_back(coord);
        }
        break;
    }

    default:break;
    }

    return pair<vector<COORDINATE>, vector<COORDINATE>>(wordCoords, limitCoords);
}

//Returns the line (row or column, depending on 'direction') that follows the coordinates (verCoord, horCoord)
string Board::getLine(char verCoord, char horCoord, char direction)
{
    string line;
    COORDINATE coordinate(verCoord, horCoord);
    vector<COORDINATE> coordsToModify;

    switch (direction) {
    case 'H':coordsToModify = generateCoordinates(nCols - horCoord + 65, coordinate, 'H').first;

        for (COORDINATE c : coordsToModify) {
            line.push_back(board[c]);
        }

        return line;
    case 'V':coordsToModify = generateCoordinates(nRows - verCoord + 65, coordinate, 'V').first;

        for (COORDINATE c : coordsToModify) {
            line.push_back(board[c]);
        }

        return line;

    default:break;
    }

    return line;
}

//Receives a pair of coordinates and changes them to the next coordinates.
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

void Board::finish()
{
    for (auto &it : board)
        if (it.second == '.')
            it.second = '#';
}

bool Board::isCurrent(string word, multimap<string, string> currentWords)
{
    for (const auto &it : currentWords) {
        if (word == it.second)
            return true;
    }

    return false;
}

// Returns the word in the perpendicular column or line
string Board::getWord(COORDINATE initialCoordinate, char direction)
{
    string word;
    COORDINATE initial = initialCoordinate;

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


