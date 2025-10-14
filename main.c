#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include <string.h>

typedef struct {
    int number;
    bool mine;
    bool open;
    bool flag;
} Cell;

int main(void) {
    int width = 10;
    int height = 10;
    Cell cells[width][height];
    memset(cells, 0, sizeof(cells));

    initscr();
    noecho();

    getch();
    endwin();
    return 0;
}
