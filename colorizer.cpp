// The following piece of code is being used with permission of the author.
// PROG - MIEIC
// JAS
// Example of a program that prints colored characters on the console (in text mode)

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include "colorizer.h"

using namespace std;

void clrscr(void)
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
        SetConsoleTextAttribute(hCon,
                                color | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
}