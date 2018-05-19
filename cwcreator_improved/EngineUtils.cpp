#include "Engine.h"

using namespace std;

// Defines the checks performed by Engine
bool Engine::coordinateInBoard(COORDINATE coordinate)
{
    for (const auto &it : board) {
        if (it.first == coordinate)
            return true;
    }

    return false;
}

bool Engine::fits(string &word, char verCoord, char horCoord, char direction)
{
    COORDINATE coordinate(verCoord, horCoord);
    switch (direction) {
    case 'V': {
        COORDINATE previous(static_cast<const char &>(coordinate.first - 1), coordinate.second);
        COORDINATE after(static_cast<const char &>(coordinate.first + word.length()), coordinate.second);

        return word.length() <= (nRows - verCoord + 65) &&
            ((!isalpha(board[after]) && !isalpha(board[previous])) || board[after] == '\0' || board[previous] == '\0');
    }
    case 'H': {
        COORDINATE previous(coordinate.first, static_cast<const char &>(coordinate.second - 1));
        COORDINATE after(coordinate.first, static_cast<const char &>(coordinate.second + word.length()));

        return word.length() <= (nRows - horCoord + 65) &&
            ((!isalpha(board[after]) && !isalpha(board[previous])) || board[after] == '\0' || board[previous] == '\0');
    }
    default: break;
    }

    return false;
}

bool Engine::matches(string word, string line)
{
    return utility.wildcardMatch(word.c_str(), line.substr(0, word.length()).c_str());
}
