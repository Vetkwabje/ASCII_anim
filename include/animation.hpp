#pragma once

#include <vector>

struct Particle {
    float x, y;
    float vx, vy;

    float lifetime;

    int color;
    char glyph;
};

class Animation {
public:
    Animation();

    void update(float deltaTime, int width, int height);

    const std::vector<Particle>& getParticles() const;

private:
    std::vector<Particle> particles;

    float spawnTimer = 0.0f;

    void spawnBurst(float x, float y);
};