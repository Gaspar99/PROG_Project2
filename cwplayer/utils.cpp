#include <iostream>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <algorithm>
#include "utils.h"
#include "Player.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////
// WildcardMatch
// str - Input string to match
// strWild - Match mask that may contain wildcards like ? and *
//
// A ? sign matches any character, except an empty string.
// A * sign matches any string inclusive an empty string.
// Characters are compared caseless.
//
// ADAPTED FROM:
// https://www.codeproject.com/Articles/188256/A-Simple-Wildcard-Matching-Function
bool wildcardMatch(const char *str, const char *strWild)
{
	// We have a special case where string is empty ("") and the mask is "*".
	// We need to handle this too. So we can't test on !*str here.
	// The loop breaks when the match string is exhausted.
	while (*strWild) {
		// Single wildcard character
		if (*strWild == '.') {
			// Matches any character except empty string
			if (!*str)
				return false;
			// OK next
			++str;
			++strWild;
		}
		else {
			// Standard compare of 2 chars. Note that *str might be 0 here,
			// but then we never get a match on *strWild
			// that has always a value while inside this loop.
			if (toupper(*str++) != toupper(*strWild++))
				return false;
		}
	}
	// Have a match? Only if both are at the end...
	return !*str && !*strWild;
}

char to_upper(int ch) {
    return static_cast<unsigned char>(toupper(ch));
}

char to_lower(int ch) {
    return static_cast<unsigned char>(tolower(ch));
}

void capitalize(string &word)
{
    transform(word.begin(), word.end(), word.begin(), [] (unsigned char c) { return toupper(c); } );
}

#if defined(_WIN32)
#include <windows.h>

// PROG - MIEIC
// JAS
// Example of a program that prints colored characters on the console (in text mode)
void clrscr()
{
    COORD coordScreen = {0, 0}; // upper left corner
    DWORD cCharsWritten;
    DWORD dwConSize;
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hCon, &csbi);
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y; // fill with spaces
    FillConsoleOutputCharacter(hCon, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
    GetConsoleScreenBufferInfo(hCon, &csbi);
    FillConsoleOutputAttribute(hCon, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
// cursor to upper left corner
    SetConsoleCursorPosition(hCon, coordScreen);
}

void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setcolor(unsigned int color)
{
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hcon, color);
}

void setcolor(unsigned int color, unsigned int background_color)
{
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    if (background_color == BLACK) SetConsoleTextAttribute(hCon, color);
    else
        SetConsoleTextAttribute(hCon, color | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
}

#elif defined(__unix__) || defined(__linux__) || defined(__APPLE__)

// PROG - MIEIC
// JAS
// Example of a program that prints colored characters on the console (in text mode)
// LINUX version, using ANSI escape sequences
#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>

  //==========================================================================================
  // Position the cursor at column 'x', line 'y'
  // The coodinates of upper left corner of the screen are (x,y)=(0,0)
  void gotoxy(int x, int y)
  {
      ostringstream oss;
      oss << "\033[" << y << ";" << x << "H";
      cout << oss.str();
  }
  //==========================================================================================
  // Clear the screen
  void clrscr()
  {
      cout << "\033[2J"; gotoxy(0, 0);
  }
  //==========================================================================================
  // Set text color
  void setcolor(string color)
  {
      cout << color;
  }
  //==========================================================================================
  // Set text color & background
  void setcolor(string color, string background_color)
  {
      cout << color << background_color;
  }

#endif