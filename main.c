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

void draw_board(int width, int height) {
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            mvprintw(y * 2    , x * 4, " --- ");
            mvprintw(y * 2 + 1, x * 4, "| # |");
            mvprintw(y * 2 + 2, x * 4, " --- ");
        }
    }
    move(1, 2);
}

void move_cursor(int x, int y) {
    move(y * 2 + 1, x * 4 + 2);
}

int main(void) {
    int width = 10;
    int height = 10;
    int sel_x = 0;
    int sel_y = 0;
    Cell cells[width][height];
    memset(cells, 0, sizeof(cells));
    bool quit = false;

    initscr();
    noecho();

    draw_board(width, height);

    while (!quit) {
        switch (getch()) {
            case 'q': quit = true; continue;
            case 'h': sel_x = sel_x - 1 >= 0 ? sel_x - 1 : width - 1; move_cursor(sel_x, sel_y); break;
            case 'l': sel_x = sel_x + 1 < width ? sel_x + 1 : 0; move_cursor(sel_x, sel_y); break;
            case 'k': sel_y = sel_y - 1 >= 0 ? sel_y - 1 : height - 1; move_cursor(sel_x, sel_y); break;
            case 'j': sel_y = sel_y + 1 < height ? sel_y + 1 : 0; move_cursor(sel_x, sel_y); break;
        }
    }

    endwin();
    return 0;
}
