#ifndef PROG_PROJECT2_UTILITY_H
#define PROG_PROJECT2_UTILITY_H

#if defined(_WIN32)
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGRAY 7
#define DARKGRAY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define WHITE 15

#elif defined(__unix__) || defined(__linux__) || defined(__APPLE__)
#define NO_COLOR "\033[0m"
#define BLACK "\033[0;30m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define BROWN "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN "\033[0;36m"
#define LIGHTGRAY "\033[0;37m"
#define DARKGRAY "\033[1;30m"
#define LIGHTRED "\033[1;31m"
#define LIGHTGREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define LIGHTBLUE "\033[1;34m"
#define LIGHTMAGENTA "\033[1;35m"
#define LIGHTCYAN "\033[1;36m"
#define WHITE "\033[1;37m"

// BACKGROUND COLOR CODES
#define BLACK_B "\033[0;40m"
#define RED_B "\033[0;41m"
#define GREEN_B "\033[0;42m"
#define YELLOW_B "\033[0;43m"
#define BLUE_B "\033[0;44m"
#define MAGENTA_B "\033[0;45m"
#define CYAN_B "\033[0;46m"
#define WHITE_B "\033[1;47m"
#endif

#include <string>

// Class Utility defines useful functions that involve manipulation of strings and chars, colorization of the
// terminal/command line and string matching.
// Should be compatible with Windows and Linux-based systems. Theoretically compatible with macOS
class Utility
{
public:
    unsigned char to_upper(int ch);
    unsigned char to_lower(int ch);
    bool wildcardMatch(const char *str, const char *strWild);
    void capitalize(std::string &word);

    void clrscr();
    void gotoxy(int x, int y);

#if defined(_WIN32)
    void setcolor(unsigned int color);
    void setcolor(unsigned int color, unsigned int background_color);

#elif defined(__unix__) || defined(__linux__) || defined(__APPLE__)
    void setcolor(std::string color);
    void setcolor(std::string color, std::string background_color);

#endif

};

#endif //PROG_PROJECT2_UTILITY_H