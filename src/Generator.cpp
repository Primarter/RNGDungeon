#include "Generator.hpp"

namespace rngd
{
    void Generator::setup(void)
    {
        InitWindow(WIDTH, HEIGHT, "RNGDungeon");
        SetTargetFPS(60);
        rlImGuiSetup(true);
        cells.setup(CellSystem::RANDOM_SEED);
    }

    void Generator::tick(void)
    {
        cells.tick();
    }

    void Generator::draw(void)
    {
        BeginDrawing();
        rlImGuiBegin();

        ClearBackground(DARKGRAY);
        cells.draw();

        rlImGuiEnd();
        EndDrawing();
    }
} // namespace rngd
