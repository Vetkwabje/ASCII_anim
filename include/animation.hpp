#pragma once

#include <vector>

struct RGBColor {
    int red;
    int green;
    int blue;
};

struct AnimationObject {
    float x, y;
    int width, height;
    RGBColor color;
    float brightness;
};

class Animation {
public:
    Animation();

    void update(float deltaTime, int width, int height);

    std::vector<AnimationObject> getObjects() const;

private:
    struct Particle {
        enum class Type {
            Rocket,
            Spark
        };

        float x, y;
        float vx, vy;

        int width, height;
        float lifetime, maxLifetime;

        RGBColor color;
        Type type;
    };

    std::vector<Particle> particles;

    float spawnTimer = 0.0f;

    void spawnRocket(int width, int height);
    void spawnExplosion(
        std::vector<Particle>& spawned,
        float x,
        float y,
        RGBColor color
    );
};