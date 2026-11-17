#pragma once

#include "animation.hpp"

class Renderer {
public:
    Renderer();

    void draw(
        const Animation& animation,
        int width,
        int height
    );
};