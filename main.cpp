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
	Puzzle puzzle1;
	puzzle1.greetUser();
	puzzle1.showMenu();

    return 0;
}