#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <ncurses.h>
#include <string.h>

typedef struct {
    int number;
    bool mine;
    bool open;
    bool flag;
} Cell;

void draw_board(int width, int height);
void place_mines(int mines, int width, int height, Cell cells[width][height]);
void calculate_numbers(int width, int height, Cell cells[width][height]);
void move_cursor(int x, int y);
void open_cell(int x, int y, int width, int height, Cell cells[width][height]);
void flag_cell(int x, int y, int width, int height, Cell cells[width][height]);

int main(void) {
    int width = 10;
    int height = 10;
    int mines = 10;
    int sel_x = 0;
    int sel_y = 0;
    Cell cells[width][height];
    memset(cells, 0, sizeof(cells));
    bool quit = false;

    initscr();
    noecho();

    place_mines(mines, width, height, cells);
    draw_board(width, height);

    while (!quit) {
        switch (getch()) {
            case 'q': quit = true; continue;
            case 'h': sel_x = sel_x - 1 >= 0 ? sel_x - 1 : width - 1; break;
            case 'l': sel_x = sel_x + 1 < width ? sel_x + 1 : 0; break;
            case 'k': sel_y = sel_y - 1 >= 0 ? sel_y - 1 : height - 1; break;
            case 'j': sel_y = sel_y + 1 < height ? sel_y + 1 : 0; break;
            case 'd': open_cell(sel_x, sel_y, width, height, cells); break;
            case 'f': flag_cell(sel_x, sel_y, width, height, cells); break;
            default: continue;
        }
        move_cursor(sel_x, sel_y);
    }

    endwin();
    return 0;
}

void move_cursor(int x, int y) {
    move(y * 2 + 1, x * 4 + 2);
}

void draw_board(int width, int height) {
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            mvprintw(y * 2    , x * 4, " --- ");
            mvprintw(y * 2 + 1, x * 4, "| # |");
            mvprintw(y * 2 + 2, x * 4, " --- ");
        }
    }
    move_cursor(0, 0);
}

void place_mines(int mines, int width, int height, Cell cells[width][height]) {
    srand(time(NULL));
    int x = 0;
    int y = 0;
    for (int i = 0; i < mines; ++i) {
        do {
            x = rand() % width;
            y = rand() % height;
        } while (cells[x][y].mine);
        cells[x][y].mine = true;
    }
}

void calculate_numbers(int width, int height, Cell cells[width][height]) {
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            if (cells[x][y].mine) continue;
            // TODO: actual logic
        }
    }
}

void open_cell(int x, int y, int width, int height, Cell cells[width][height]) {
    if (x < 0 || y < 0 || x >= width || y >= height) {
        return;
    }

    if (cells[x][y].flag || cells[x][y].open) {
        return;
    }

    cells[x][y].open = true;
    if (cells[x][y].number == 0) {
        printw(" ");
        // TODO: open surrounding cells
    } else {
        printw("%d", cells[x][y].number);
    }
}

void flag_cell(int x, int y, int width, int height, Cell cells[width][height]) {
    if (cells[x][y].open) {
        return;
    }

    if (cells[x][y].flag) {
        cells[x][y].flag = false;
        printw("#");
    } else {
        cells[x][y].flag = true;
        printw("F");
    }
}
