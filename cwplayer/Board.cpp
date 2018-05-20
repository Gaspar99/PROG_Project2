#include <iostream>
#include <iomanip>
#include <algorithm>
#include "Board.h"
#include "utils.h"
#include "Player.h"
#include "Dictionary.h"

using namespace std;
using namespace BoardTypes;

Board::Board(unsigned int nRows, unsigned int nColumns)
{
    this->nRows = nRows;
    this->nCols = nColumns;

    // Initializes all fields of the board map with all possible Coordinates and dots.
    // The board map itself doesn't have any letters until a word is added.
    for (unsigned int i = 0; i < nRows; ++i) {
        for (unsigned int j = 0; j < nCols; ++j) {
            // builds a string like "Aa", "Ab", etc
            board.insert(pair<COORDINATE, char>(COORDINATE(static_cast<const char &>(i + 65),
                                                           static_cast<const char &>(j + 65)), '#'));
        }
    }
}

void Board::reset()
{
    for (unsigned int i = 0; i < nRows; ++i) {
        for (unsigned int j = 0; j < nCols; ++j) {
            COORDINATE coord(static_cast<const char &>(i + 65), static_cast<const char &>(j + 65));

            if (board[coord] != '#')
                board[coord] = '.';
        }
    }
}

ostream &operator<<(ostream &out, Board &board)
{
    out << endl;

    // Prints the first line of the board, which corresponds to the letters of the columns.
    for (size_t i = 0; i < board.nCols; ++i) {
        if (i == 0) {
            setcolor(CYAN);
            out << setw(4) << right << string(1, char(i + 97));
        }
        else {
            out << setw(2) << string(1, char(i + 97));
        }
    }

    out << endl;

    // Prints the remainder of the lines.
    for (size_t i = 0; i < board.nRows; ++i) {
        setcolor(CYAN);
        out << setw(2) << left << string(1, char(i + 65)) << right << setw(1); // Prints the firstCoord letter.

        for (size_t j = 0; j < board.nCols; ++j) {
            COORDINATE c(char(i + 65), char(j + 65));

            if (board.board[c] == '#' && c != pair<char, char>(char(i), char(i + 32))) {
                out << " ";

#if defined(_WIN32)
                setcolor(BLACK, WHITE);
#elif defined(__unix__) || defined(__linux__) || defined(__APPLE__)
                setcolor(BLACK, WHITE_B);
#endif

                out << setw(1) << '#';
            }
            else if (board.board[c] == '#') {
#if defined(_WIN32)
                setcolor(BLACK, WHITE);
#elif defined(__unix__) || defined(__linux__) || defined(__APPLE__)
                setcolor(BLACK, WHITE_B);
#endif

                out << left << setw(1) << '#' << right;
            }
            else {
                setcolor(WHITE);
                out << setw(2) << board.board[c];
            }
        }

        out << endl;
    }

    out << endl;

    return out;
}

void Board::modifyMap(const string &word, COORDINATE initialCoord, char direction, int mode)
{
    size_t wordLength = word.length();
    vector<COORDINATE> coordsToModify = generateCoords(wordLength, initialCoord, direction);

    if (direction == 'H') { direction = 'V'; } else if (direction == 'V') { direction = 'H'; };

    for (unsigned int i = 0; i < wordLength; ++i) {
        COORDINATE currentCoord = coordsToModify[i];
        char &currentLetter = board[currentCoord];

        string wordLine = getWord(currentCoord, direction);

        if ((!isalpha(currentLetter) && mode == 0) || (!Dictionary::isValid(wordLine) && mode == 1))
            currentLetter = word[i];
    }
}

void Board::removeWord(COORDINATE initialCoord, char direction)
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

        return modifyMap(word, initialCoord, direction, 1);
    }

    case 'V': {
        unsigned int i = 0;

        while (initialCoord.first + i <= (nRows + 65)) {
            COORDINATE next(static_cast<const char &>(initialCoord.first + i), initialCoord.second);
            if (board[next] == '#') break;
            ++i;
        }

        string word = string(i, '.');

        return modifyMap(word, initialCoord, direction, 1);
    }
    default:break;
    }
}

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

void Board::addWord(const string &word, COORDINATE initialCoord, char direction)
{
    return modifyMap(word, initialCoord, direction);
}

vector<COORDINATE> Board::generateCoords(unsigned int wordLength, COORDINATE initialCoord, char direction)
{
    vector<COORDINATE> wordCoords;
    vector<COORDINATE> limitCoords;

    switch (direction) {
    case 'V': {
        int j = 0;
        for (char i = initialCoord.first; j < wordLength; ++i, ++j) {
            COORDINATE coord(i, initialCoord.second);
            wordCoords.push_back(coord);
        }
        break;
    }

    case 'H': {
        int j = 0;
        for (char i = initialCoord.second; j < wordLength; ++i, ++j) {
            COORDINATE coord(initialCoord.first, i);
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
    COORDINATE coordinate(verCoord, horCoord);
    vector<COORDINATE> coordsToModify;

    switch (direction) {
    case 'H': coordsToModify = generateCoords(nCols - horCoord + 65, coordinate, 'H');

        for (COORDINATE c : coordsToModify) {
            line.push_back(board[c]);
        }

        return line;
    case 'V': coordsToModify = generateCoords(nRows - verCoord + 65, coordinate, 'V');

        for (COORDINATE c : coordsToModify) {
            line.push_back(board[c]);
        }

        return line;

    default:break;
    }

    return line;
}

void Board::insertWhiteCells(string word, COORDINATE initialCoord, char direction)
{
    vector<COORDINATE> coordsToModify = generateCoords(word.length(), initialCoord, direction);

    for (const auto &i : coordsToModify) {
        board[i] = '.';
    }
}

