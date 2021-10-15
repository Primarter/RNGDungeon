#include "stdInclude.hpp"
#include "lib/Lib.hpp"
#include "Generator.hpp"

int main(void)
{
    rngd::Generator generator;

    generator.setup();

    while (!WindowShouldClose())
    {
        generator.update();
        generator.draw();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}
