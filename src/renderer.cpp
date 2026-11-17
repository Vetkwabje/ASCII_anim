#include "renderer.hpp"

#include <ncurses.h>

Renderer::Renderer() {
    if (has_colors()) {
        start_color();
        use_default_colors();

        init_pair(1, COLOR_RED, -1);
        init_pair(2, COLOR_GREEN, -1);
        init_pair(3, COLOR_YELLOW, -1);
        init_pair(4, COLOR_BLUE, -1);
        init_pair(5, COLOR_MAGENTA, -1);
        init_pair(6, COLOR_CYAN, -1);
    }
}

void Renderer::draw(
    const Animation& animation,
    int width,
    int height
) {
    erase();

    for (const auto& p : animation.getParticles()) {
        int x = static_cast<int>(p.x);
        int y = static_cast<int>(p.y);

        if (x < 0 || x >= width || y < 0 || y >= height) {
            continue;
        }

        mvaddch(
            y,
            x,
            p.glyph | COLOR_PAIR(p.color)
        );
    }

    refresh();
}