#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <ctime>
#include <chrono>
#include "Board.h"
#include "colorizer.h"

using namespace std;
using namespace std::chrono;

//TODO: We're gonna need this eventually
//Takes a string and puts all its characters in uppercase
void capitalize(string &word)
{
    transform(word.begin(), word.end(), word.begin(), [] (unsigned char c) { return toupper(c); } );
}

int main()
{
    Board b(10,10);

    Board::coord a('A', 'a');

    b.showBoard();

    b.showBoard();

    return 0;
}