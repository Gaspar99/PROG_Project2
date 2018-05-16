#include <iostream>
#include <ctime>
#include <chrono>
#include "Board.h"
#include "utils.h"
#include "Puzzle.h"

using namespace std;
using namespace std::chrono;

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));
	Puzzle puzzle1;

    puzzle1.greetUser();

    while(true) {
        puzzle1.showMenu();
        exit(0);
    }
}