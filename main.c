#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>

void draw_board(int width, int height) {
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            mvprintw(y * 2,     x * 4, " --- ");
            mvprintw(y * 2 + 1, x * 4, "|   |");
            mvprintw(y * 2 + 2, x * 4, " --- ");
        }
    }
}

void draw_cells(int width, int height, bool *flags) {
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            if (flags[x + y * width]) {
                mvprintw(y * 2 + 1, x * 4 + 2, "F");
                continue;
            }
            mvprintw(y * 2 + 1, x * 4 + 2, "#");
        }
    }
}

void place_mines(int total_mines, int total_cells, bool *mines) {
    for (int i = 0; i < total_cells; ++i) {
        mines[i] = false;
    }

    int sel = 0;
    for (int i = 0; i < total_mines; ++i) {
        do {
            sel = rand() % total_cells;
        } while (mines[sel]);
        mines[sel] = true;
    }
}

int main() {
    int height = 10;
    int width = 10;
    int total_mines = 10;

    int pos[2] = {0};
    int inp = 0;
    int total_cells = height * width;
    bool *mines = malloc(sizeof(bool[total_cells]));
    bool *flags = malloc(sizeof(bool[total_cells]));
    for (int i = 0; i < total_cells; ++i) {
        flags[i] = false;
    }

    bool quit = false;

    place_mines(total_mines, total_cells, mines);
    
    initscr();
    noecho();

    draw_board(width, height);
    draw_cells(width, height, flags);
    move(pos[1] * 2 + 1, pos[0] * 4 + 2);

    while (!quit) {
        inp = getch();
        switch (inp) {
            case 'q': quit = true; break;
            case 'h': pos[0] = (pos[0] - 1) < 0 ? width - 1 : pos[0] - 1; break;
            case 'l': pos[0] = (pos[0] + 1) > width - 1 ? 0 : pos[0] + 1; break;
            case 'k': pos[1] = (pos[1] - 1) < 0 ? height - 1 : pos[1] - 1; break;
            case 'j': pos[1] = (pos[1] + 1) > height - 1 ? 0 : pos[1] + 1; break;
            case 'f': flags[pos[0] + pos[1] * width] = !flags[pos[0] + pos[1] * width]; break;
        }
        draw_cells(width, height, flags);
        move(pos[1] * 2 + 1, pos[0] * 4 + 2);
    }
   
    endwin();

    free(mines);
    return 0;
}
