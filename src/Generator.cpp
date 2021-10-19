#include "Generator.hpp"

namespace rngd
{
    void Generator::setup(void)
    {
        InitWindow(WIDTH, HEIGHT, "RNGDungeon");
        SetTargetFPS(60);
        rlImGuiSetup(true);
        genCellSys.setup(GenCellSystem::RANDOM_SEED);
    }

    void Generator::stop(void)
    {
        rlImGuiShutdown();
        CloseWindow();
    }

    void Generator::reset(void)
    {
        genCellSys.reset(GenCellSystem::RANDOM_SEED);
    }

    void Generator::update(void)
    {
        if (!genCellSys.separated)
            genCellSys.separate();
        else
            genCellSys.updateGraph();
    }

    void Generator::draw(void)
    {
        BeginDrawing();
        rlImGuiBegin();

        this->drawImGui();
        this->drawGen();

        rlImGuiEnd();
        EndDrawing();
    }

    void Generator::drawImGui(void)
    {
        bool open = false;
        ImGui::Begin("Settings", &open);
        if (ImGui::Button("Restart"))
            this->reset();

        ImGui::End();
    }

    void Generator::drawGen(void)
    {
        ClearBackground(DARKGRAY);
        genCellSys.draw();
    }
} // namespace rngd
