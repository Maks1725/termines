#include <ncurses.h>

int main() {
    int height = 5;
    int width = 10;
    
    initscr();

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            mvprintw(y * 2,     x * 4, "+---+");
            mvprintw(y * 2 + 1, x * 4, "|   |");
            mvprintw(y * 2 + 2, x * 4, "'---'");
        }
    }
    
    getch();
    endwin();
    return 0;
}
