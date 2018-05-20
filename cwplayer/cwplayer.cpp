#include "Puzzle.h"
#include "utils.h"
#include <iostream>
#include <chrono>
#include <ctime>

using namespace std;

int main()
{
    clrscr();
    srand(static_cast<unsigned int>(time(NULL)));
    Puzzle puzzle1;

    setcolor(WHITE);
    puzzle1.greetUser();

    while (true) {
        puzzle1.showMenu();
        exit(0);
    }
}


