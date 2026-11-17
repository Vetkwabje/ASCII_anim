#include "terminal.hpp"

#include <ncurses.h>

Terminal::Terminal() {
    initscr();

    cbreak();
    noecho();

    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    curs_set(0);
}

Terminal::~Terminal() {
    endwin();
}

int Terminal::readKey() {
    return getch();
}

std::pair<int, int> Terminal::getSize() const {
    int height, width;

    getmaxyx(stdscr, height, width);

    return {width, height};
}