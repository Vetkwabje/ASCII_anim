#pragma once

#include "animation.hpp"

#include <unordered_map>
#include <vector>

class Renderer {
public:
    Renderer();

    void draw(
        const std::vector<AnimationObject>& objects,
        int width,
        int height
    );

private:
    short colorPairFor(const RGBColor& color);

    std::unordered_map<int, short> colorPairs;
    short nextColor = 16;
    short nextPair = 1;
    bool rgbSupported = false;
};