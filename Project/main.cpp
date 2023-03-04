#include "glm/fwd.hpp"
#include <stdlib.h> 
#include "p6/p6.h"
#include "Boid.h"

int main()
{
    auto ctx = p6::Context{{1280, 720, "Project R"}};

    // INITIALIZATION CODE

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    std::vector<Boid> boids;

    for (int i = 0; i < 50; ++i) {
        Boid boid(glm::vec2(dist(mt), dist(mt)), glm::vec2(0.001, -0.0005));
        boids.push_back(boid);
    }

    ctx.fill = {1, 1, 1};
    // UPDADE LOOP
    ctx.update = [&]() {
        ctx.background({});

        for (Boid &boid : boids) { 
            boid.update(ctx, boids);
            boid.draw(ctx);
        }

        //borders
        ctx.stroke = p6::Color{1.f, 1.f, 1.f};
        ctx.stroke_weight = 0.01f;
        ctx.line(glm::vec2{-1, 1}, glm::vec2{1, 1});
        ctx.line(glm::vec2{-1, -1}, glm::vec2{1, -1});
        ctx.circle(p6::Center{glm::vec2{0}}, p6::Radius{0.01f});
        
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}