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
void update_counter(int mines_left, int width);
void win(int width);
void lose(int width, int height, Cell cells[width][height]);
void place_mines(int mines, int width, int height, Cell cells[width][height]);
void calculate_numbers(int width, int height, Cell cells[width][height]);
void move_cursor(int x, int y);
void open_cell(int x, int y, int *opened_cells, int width, int height, Cell cells[width][height]);
void open_neighbors(int x, int y, int *opended_cells, int width, int height, Cell cells[width][height]);
void flag_cell(int x, int y, int *flags, int width, int height, Cell cells[width][height]);
int get_neighbor_flags(int x, int y, int width, int height, Cell cells[width][height]);
bool is_mine(int x, int y, int width, int height, Cell cells[width][height]);
bool is_flag(int x, int y, int width, int height, Cell cells[width][height]);
void set_colors();

int main(void) {
    // TODO: specify board size and mines in arguments, time
    const int width = 10;
    const int height = 10;
    const int mines = 10;
    int sel_x = 0;
    int sel_y = 0;
    int flags = 0;
    int opened_cells = 0;
    const int total_cells = width * height;
    Cell cells[width][height];
    memset(cells, 0, sizeof(cells));
    bool quit = false;

    initscr();
    noecho();
    set_colors();

    place_mines(mines, width, height, cells);
    calculate_numbers(width, height, cells);
    draw_board(width, height);

    while (!quit) {
        if (opened_cells + mines == total_cells) {
            win(width);
        }
        update_counter(mines - flags, width);
        move_cursor(sel_x, sel_y);
        switch (getch()) {
            case 'q': quit = true; continue;
            case 'h': sel_x = sel_x - 1 >= 0 ? sel_x - 1 : width - 1; break;
            case 'l': sel_x = sel_x + 1 < width ? sel_x + 1 : 0; break;
            case 'k': sel_y = sel_y - 1 >= 0 ? sel_y - 1 : height - 1; break;
            case 'j': sel_y = sel_y + 1 < height ? sel_y + 1 : 0; break;
            case 'd':
                if (cells[sel_x][sel_y].open && cells[sel_x][sel_y].number > 0 && cells[sel_x][sel_y].number == get_neighbor_flags(sel_x, sel_y, width, height, cells)) {
                    open_neighbors(sel_x, sel_y, &opened_cells, width, height, cells);
                } else {
                    open_cell(sel_x, sel_y, &opened_cells, width, height, cells);
                }
                break;
            case 'f': flag_cell(sel_x, sel_y, &flags, width, height, cells); break;
            default: continue;
        }
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
            attron(COLOR_PAIR(9));
            mvprintw(y * 2    , x * 4, " --- ");
            mvprintw(y * 2 + 1, x * 4, "|   |");
            mvprintw(y * 2 + 2, x * 4, " --- ");
            attroff(COLOR_PAIR(9));
            attron(COLOR_PAIR(0));
            mvprintw(y * 2 + 1, x * 4 + 2, "#");
            attroff(COLOR_PAIR(0));
        }
    }
    mvprintw(1, 4 * width + 3, "Mines left:");
    mvprintw(5, 4 * width + 3, "Controls:");
    mvprintw(6, 4 * width + 3, "Movement - hjkl");
    mvprintw(7, 4 * width + 3, "Open cell - d");
    mvprintw(8, 4 * width + 3, "Flag cell - f");
}

void update_counter(int mines_left, int width) {
    mvprintw(1, 4 * width + 15, "%3d", mines_left);
}

void win(int width) {
    mvprintw(3, 4 * width + 3, "You Win!");
}

void lose(int width, int height, Cell cells[width][height]) {
    mvprintw(3, 4 * width + 3, "You Lose!");
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            if (cells[x][y].mine) {
                attron(COLOR_PAIR(11));
                mvprintw(y * 2 + 1, x * 4 + 2, "X");
                attroff(COLOR_PAIR(11));
            }
        }
    }
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

bool is_mine(int x, int y, int width, int height, Cell cells[width][height]) {
    if (x < 0 || y < 0 || x >= width || y >= height) {
        return false;
    }
    return cells[x][y].mine;
}

bool is_flag(int x, int y, int width, int height, Cell cells[width][height]) {
    if (x < 0 || y < 0 || x >= width || y >= height) {
        return false;
    }
    return cells[x][y].flag;
}

void calculate_numbers(int width, int height, Cell cells[width][height]) {
    int m;
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            if (cells[x][y].mine) continue;
            m = 0;
            m += is_mine(x - 1, y - 1, width, height, cells) + is_mine(x, y - 1, width, height, cells) + is_mine(x + 1, y - 1, width, height, cells);
            m += is_mine(x - 1, y, width, height, cells) + is_mine(x + 1, y, width, height, cells);
            m += is_mine(x - 1, y + 1, width, height, cells) + is_mine(x, y + 1, width, height, cells) + is_mine(x + 1, y + 1, width, height, cells);
            cells[x][y].number = m;
        }
    }
}

void open_neighbors(int x, int y, int *opened_cells, int width, int height, Cell cells[width][height]) {
    open_cell(x - 1, y - 1, opened_cells, width, height, cells);
    open_cell(x - 1, y, opened_cells, width, height, cells);
    open_cell(x - 1, y + 1, opened_cells, width, height, cells);
    open_cell(x, y - 1, opened_cells, width, height, cells);
    open_cell(x, y + 1, opened_cells, width, height, cells);
    open_cell(x + 1, y - 1, opened_cells, width, height, cells);
    open_cell(x + 1, y, opened_cells, width, height, cells);
    open_cell(x + 1, y + 1, opened_cells, width, height, cells);
}

void open_cell(int x, int y, int *opened_cells, int width, int height, Cell cells[width][height]) {
    if (x < 0 || y < 0 || x >= width || y >= height) {
        return;
    }

    if (cells[x][y].flag || cells[x][y].open) {
        return;
    }

    if (cells[x][y].mine) {
        lose(width, height, cells);
        return;
    }

    cells[x][y].open = true;
    (*opened_cells)++;
    move_cursor(x, y);
    if (cells[x][y].number == 0) {
        printw(" ");
        open_neighbors(x, y, opened_cells, width, height, cells);
    } else {
        attron(COLOR_PAIR(cells[x][y].number));
        printw("%d", cells[x][y].number);
        attroff(COLOR_PAIR(cells[x][y].number));
    }
}

void flag_cell(int x, int y, int *flags, int width, int height, Cell cells[width][height]) {
    if (cells[x][y].open) {
        return;
    }

    if (cells[x][y].flag) {
        cells[x][y].flag = false;
        (*flags)--;
        attron(COLOR_PAIR(0));
        printw("#");
        attroff(COLOR_PAIR(0));
    } else {
        cells[x][y].flag = true;
        (*flags)++;
        attron(COLOR_PAIR(10));
        printw("@");
        attroff(COLOR_PAIR(10));
    }
}

int get_neighbor_flags(int x, int y, int width, int height, Cell cells[width][height]) {
    int flags = 0;
    flags += is_flag(x - 1, y - 1, width, height, cells) + is_flag(x, y - 1, width, height, cells) + is_flag(x + 1, y - 1, width, height, cells);
    flags += is_flag(x - 1, y, width, height, cells) + is_flag(x + 1, y, width, height, cells);
    flags += is_flag(x - 1, y + 1, width, height, cells) + is_flag(x, y + 1, width, height, cells) + is_flag(x + 1, y + 1, width, height, cells);
    return flags;
}

void set_colors() {
    // TODO: check if terminal supports colors
    start_color();
    use_default_colors();

    init_color(0, 800, 800, 800); // Closed cells
    init_color(9, 400, 400, 400);    // Cell borders
    init_color(10, 800, 800, 400);   // Flags
    init_color(11, 800, 0, 0);      // Mines

    // Colors 1..8 are for numbers
    init_color(1, 400, 400, 800);       // Blue
    init_color(2, 400, 800, 400);       // Green
    init_color(3, 800, 400, 400);       // Red
    init_color(4, 200, 200, 600);        // Dark blue
    init_color(5, 600, 600, 200);      // Maroon
    init_color(6, 200, 600, 600);      // Cyan
    init_color(7, 400, 400, 400);    // Gray
    init_color(8, 800, 800, 800); // White

    // Color pairs
    init_pair(0, 0, -1);
    init_pair(1, 1, -1);
    init_pair(2, 2, -1);
    init_pair(3, 3, -1);
    init_pair(4, 4, -1);
    init_pair(5, 5, -1);
    init_pair(6, 6, -1);
    init_pair(7, 7, -1);
    init_pair(8, 8, -1);
    init_pair(9, 9, -1);
    init_pair(10, 10, -1);
    init_pair(11, 11, -1);

    attron(A_BOLD);
}
