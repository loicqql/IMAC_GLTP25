#include "p6/p6.h"

#include <vector>
#include <cmath>

struct xy {
    float x;
    float y;
    float dx;
    float dy;
    float r;
    float g;
    float b;
};

float RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

float resetVec(std::vector<xy> &vec) {
    vec.clear();
    for (int i = 0; i < 100; ++i) {
        xy coords{};
        coords.x = RandomFloat(-1.0f, 1.0f);
        coords.y = RandomFloat(-1.0f, 1.0f);
        coords.dx = RandomFloat(-0.003f, 0.003f);
        coords.dy = RandomFloat(-0.003f, 0.003f);
        coords.r = RandomFloat(0.0f, 1.0f);
        coords.g = RandomFloat(0.0f, 1.0f);
        coords.b = RandomFloat(0.0f, 1.0f);
        vec.push_back(coords);
    }
}

int main()
{
    auto ctx = p6::Context{{1280, 720, "TP3 EX1"}};
    ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    std::vector<xy> vec;

    resetVec(vec);

    // Declare your infinite update loop.
    ctx.update = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        auto nb = 50;

        float rot = 0.0f;

        ImGui::Begin("Test");
        ImGui::SliderInt("nb", &nb, 5, 100);
        ImGui::SliderFloat("nb", &rot, 0, 3.14f);
        ImGui::End();        // Show the official ImGui demo window        // It is very useful to discover all the widgets available in ImGui        ImGui::ShowDemoWindow();


        ctx.background(p6::NamedColor::Black);
        // ctx.circle(p6::Center{ctx.mouse()}, p6::Radius{0.2f});

        // ctx.square(p6::Center{glm::vec2(ctx.current_canvas_width() / 2, ctx.current_canvas_height() / 2)}, p6::Radius{0.1f});

        for (int i = 0; i < nb; ++i) {
            xy coords = vec[i];
            ctx.fill = {coords.r, coords.g, coords.b};
            ctx.equilateral_triangle(p6::Center{glm::vec2(coords.x, coords.y)}, p6::Radius(0.2), p6::Rotation(p6::Radians(rot)));
        }

        for (xy &coords : vec) {
            coords.x += coords.dx;
            coords.y += coords.dy;
        }

        ctx.mouse_pressed = [&vec](p6::MouseButton button) {
            resetVec(vec);
        };
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}