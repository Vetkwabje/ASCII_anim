#include "renderer.hpp"

#include <ncurses.h>
#include <algorithm>
#include <vector>

namespace {
    constexpr char BRIGHTNESS[] = " .:-=+*#%@";
    constexpr int BRIGHTNESS_LEVELS = sizeof(BRIGHTNESS) - 1;
    constexpr int RGB_LEVELS = 6;
    constexpr int RGB_STEP = 255 / (RGB_LEVELS - 1);

    int colorKey(const RGBColor& color) {
        return (color.red << 16) | (color.green << 8) | color.blue;
    }

    RGBColor quantize(const RGBColor& color) {
        auto quantizeChannel = [](int channel) {
            return (channel / RGB_STEP) * RGB_STEP;
        };

        return {
            quantizeChannel(color.red),
            quantizeChannel(color.green),
            quantizeChannel(color.blue)
        };
    }
}

Renderer::Renderer() {
    if (has_colors()) {
        start_color();
        use_default_colors();

        rgbSupported =
            can_change_color() &&
            COLORS >= 16 + RGB_LEVELS * RGB_LEVELS * RGB_LEVELS &&
            COLOR_PAIRS > 1 + RGB_LEVELS * RGB_LEVELS * RGB_LEVELS;
    }
}

short Renderer::colorPairFor(const RGBColor& color) {
    if (!rgbSupported) {
        return 0;
    }

    RGBColor quantized = quantize(color);
    int key = colorKey(quantized);

    auto existing = colorPairs.find(key);
    if (existing != colorPairs.end()) {
        return existing->second;
    }

    if (nextColor >= COLORS || nextPair >= COLOR_PAIRS) {
        return 0;
    }

    init_color(
        nextColor,
        quantized.red * 1000 / 255,
        quantized.green * 1000 / 255,
        quantized.blue * 1000 / 255
    );
    init_pair(nextPair, nextColor, -1);

    colorPairs.emplace(key, nextPair);
    ++nextColor;
    return nextPair++;
}

void Renderer::draw(
    const std::vector<AnimationObject>& objects,
    int width,
    int height
) {
    struct Pixel {
        char glyph = ' ';
        int red = 0;
        int green = 0;
        int blue = 0;
        int colorCount = 0;
        float brightness = 0.0f;
    };

    std::vector<std::vector<Pixel>> plane(
        height,
        std::vector<Pixel>(width)
    );

    for (const auto& object : objects) {
        int x = static_cast<int>(object.x);
        int y = static_cast<int>(object.y);

        for (int objectY = 0; objectY < object.height; ++objectY) {
            for (int objectX = 0; objectX < object.width; ++objectX) {
                int pixelX = x + objectX;
                int pixelY = y + objectY;

                if (pixelX < 0 || pixelX >= width ||
                    pixelY < 0 || pixelY >= height) {
                    continue;
                }

                Pixel& pixel = plane[pixelY][pixelX];
                pixel.red += object.color.red;
                pixel.green += object.color.green;
                pixel.blue += object.color.blue;
                ++pixel.colorCount;
                pixel.brightness = std::max(
                    pixel.brightness,
                    object.brightness
                );
            }
        }
    }

    erase();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const Pixel& pixel = plane[y][x];
            if (pixel.colorCount == 0) {
                mvaddch(y, x, ' ');
                continue;
            }

            int brightnessIndex = static_cast<int>(
                std::clamp(pixel.brightness, 0.0f, 1.0f) *
                (BRIGHTNESS_LEVELS - 1)
            );
            RGBColor mixedColor = {
                pixel.red / pixel.colorCount,
                pixel.green / pixel.colorCount,
                pixel.blue / pixel.colorCount
            };

            mvaddch(
                y,
                x,
                BRIGHTNESS[brightnessIndex] |
                    COLOR_PAIR(colorPairFor(mixedColor))
            );
        }
    }

    refresh();
}
