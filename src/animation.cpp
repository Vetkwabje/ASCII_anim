#include "animation.hpp"

#include <algorithm>
#include <cmath>
#include <random>

namespace {
    constexpr float PI = 3.14159265f;
    constexpr float GRAVITY = 5.0f;

    std::mt19937 rng(std::random_device{}());
}

Animation::Animation() = default;

void Animation::spawnRocket(int width, int height) {
    std::uniform_real_distribution<float> xPosition(
        width * 0.1f,
        width * 0.9f
    );
    std::uniform_real_distribution<float> angle(-0.35f, 0.35f);
    std::uniform_real_distribution<float> speed(8.0f, 11.0f);
    std::uniform_int_distribution<int> channel(0, 255);

    float launchAngle = -PI / 2.0f + angle(rng);
    float launchSpeed = speed(rng);

    Particle rocket;
    rocket.x = xPosition(rng);
    rocket.y = static_cast<float>(height) + 1.0f;
    rocket.vx = std::cos(launchAngle) * launchSpeed;
    rocket.vy = std::sin(launchAngle) * launchSpeed;
    rocket.width = 1;
    rocket.height = 1;
    rocket.lifetime = 8.0f;
    rocket.maxLifetime = rocket.lifetime;
    rocket.color = {
        channel(rng),
        channel(rng),
        channel(rng)
    };
    rocket.type = Particle::Type::Rocket;

    particles.push_back(rocket);
}

void Animation::spawnExplosion(
    std::vector<Particle>& spawned,
    float x,
    float y,
    RGBColor color
) {
    std::uniform_real_distribution<float> angle(0.0f, 2.0f * PI);
    std::uniform_real_distribution<float> speed(2.0f, 6.0f);
    std::uniform_real_distribution<float> life(1.0f, 2.4f);

    for (int i = 0; i < 45; ++i) {
        float a = angle(rng);
        float s = speed(rng);

        Particle particle;
        particle.x = x;
        particle.y = y;
        particle.vx = std::cos(a) * s;
        particle.vy = std::sin(a) * s;
        particle.width = 1;
        particle.height = 1;
        particle.lifetime = life(rng);
        particle.maxLifetime = particle.lifetime;
        particle.color = color;
        particle.type = Particle::Type::Spark;

        spawned.push_back(particle);
    }
}

void Animation::update(float deltaTime, int width, int height) {
    spawnTimer += deltaTime;

    if (spawnTimer >= 1.5f) {
        spawnTimer = 0.0f;
        spawnRocket(width, height);
    }

    std::vector<Particle> spawned;

    for (auto& particle : particles) {
        float previousVy = particle.vy;

        particle.x += particle.vx * deltaTime;
        particle.y += particle.vy * deltaTime;
        particle.vy += GRAVITY * deltaTime;
        particle.lifetime -= deltaTime;

        if (particle.type == Particle::Type::Rocket &&
            previousVy < 0.0f &&
            particle.vy >= 0.0f) {
            spawnExplosion(
                spawned,
                particle.x,
                particle.y,
                particle.color
            );
            particle.lifetime = 0.0f;
        }
    }

    particles.insert(particles.end(), spawned.begin(), spawned.end());

    particles.erase(
        std::remove_if(
            particles.begin(),
            particles.end(),
            [width, height](const Particle& particle) {
                return particle.lifetime <= 0.0f ||
                       (particle.type == Particle::Type::Spark &&
                        (particle.x + particle.width <= 0 ||
                         particle.x >= width ||
                         particle.y + particle.height <= 0 ||
                         particle.y >= height));
            }
        ),
        particles.end()
    );
}

std::vector<AnimationObject> Animation::getObjects() const {
    std::vector<AnimationObject> objects;
    objects.reserve(particles.size());

    for (const auto& particle : particles) {
        objects.push_back({
            particle.x,
            particle.y,
            particle.width,
            particle.height,
            particle.color,
            particle.type == Particle::Type::Rocket
                ? 1.0f
                : particle.lifetime / particle.maxLifetime
        });
    }

    return objects;
}