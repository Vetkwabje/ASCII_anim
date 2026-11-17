#include "animation.hpp"

#include <algorithm>
#include <cmath>
#include <random>

namespace {
    constexpr float PI = 3.14159265f;

    std::mt19937 rng(std::random_device{}());
}

Animation::Animation() {
    spawnBurst(40, 12);
}

void Animation::spawnBurst(float x, float y) {
    std::uniform_real_distribution<float> angle(0.0f, 2.0f * PI);
    std::uniform_real_distribution<float> speed(0.5f, 3.0f);
    std::uniform_int_distribution<int> color(1, 6);
    std::uniform_int_distribution<int> life(20, 60);

    for (int i = 0; i < 35; i++) {
        float a = angle(rng);
        float s = speed(rng);

        Particle p;

        p.x = x;
        p.y = y;

        p.vx = std::cos(a) * s;
        p.vy = std::sin(a) * s;

        p.lifetime = life(rng) / 30.0f;

        p.color = color(rng);
        p.glyph = '*';

        particles.push_back(p);
    }
}

void Animation::update(float deltaTime, int width, int height) {
    spawnTimer += deltaTime;

    if (spawnTimer >= 2.0f) {
        spawnTimer = 0.0f;

        spawnBurst(
            static_cast<float>(width) / 2.0f,
            static_cast<float>(height) / 2.0f
        );
    }

    for (auto& p : particles) {
        p.x += p.vx * deltaTime;
        p.y += p.vy * deltaTime;

        // Gravity
        p.vy += 0.8f * deltaTime;

        p.lifetime -= deltaTime;
    }

    particles.erase(
        std::remove_if(
            particles.begin(),
            particles.end(),
            [width, height](const Particle& p) {
                return p.lifetime <= 0.0f ||
                       p.x < 0 || p.x >= width ||
                       p.y < 0 || p.y >= height;
            }
        ),
        particles.end()
    );
}

const std::vector<Particle>& Animation::getParticles() const {
    return particles;
}