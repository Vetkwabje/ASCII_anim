#include "animation.hpp"
#include "renderer.hpp"
#include "terminal.hpp"

#include <chrono>
#include <thread>

int main() {
    Terminal terminal;
    Renderer renderer;
    Animation animation;

    constexpr float deltaTime = 1.0f / 30.0f;

    while (true) {
        int key = terminal.readKey();

        if (key == 'q' || key == 'Q') {
            break;
        }

        auto [width, height] = terminal.getSize();

        animation.update(deltaTime, width, height);
        renderer.draw(animation, width, height);

        std::this_thread::sleep_for(
            std::chrono::milliseconds(33)
        );
    }

    return 0;
}