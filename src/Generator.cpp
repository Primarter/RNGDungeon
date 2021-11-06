#include "Generator.hpp"

namespace rngd
{
    Generator::Generator(void)
    {
         this->fileName.resize(15);
    }
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

    void Generator::exportToFile(void) {
        std::cout << this->fileName << std::endl;
        auto grid = genCellSys.exportGrid();
        for (auto &&row : grid.cells) {
            for (auto &&cell : row) {
                switch (cell.type)
                {
                case ROOM:
                    std::cout << "# ";
                    break;
                case CORRIDOR:
                    std::cout << "¤ ";
                    break;
                default:
                    std::cout << ". ";
                    break;
                }
            }
            std::cout << std::endl;
        }
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
        if (ImGui::Button("Export"))
            this->exportToFile();
        ImGui::Text("Filename for export:");
        ImGui::InputText("", &fileName[0], fileName.size());

        ImGui::End();
    }

    void Generator::drawGen(void)
    {
        ClearBackground(DARKGRAY);
        genCellSys.draw();
    }
} // namespace rngd
