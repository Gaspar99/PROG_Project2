#ifndef PROG_PROJECT2_COLORIZER_H
#define PROG_PROJECT2_COLORIZER_H

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

void clrscr(void);
void gotoxy(int x, int y);
void setcolor(unsigned int color);
void setcolor(unsigned int color, unsigned int background_color);

#endif //PROG_PROJECT2_COLORIZER_H
