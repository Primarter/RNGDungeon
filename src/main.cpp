#include "stdInclude.hpp"
#include "lib/Lib.hpp"
#include "CellSystem.hpp"
#include "Generator.hpp"

int main(void)
{
    rngd::Generator generator;

    generator.setup();

    while (!WindowShouldClose())
    {
        generator.tick();
        generator.draw();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}
