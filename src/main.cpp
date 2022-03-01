#include "stdInclude.hpp"
#include "lib/Lib.hpp"
#include "Generator.hpp"

std::ostream &operator<<(std::ostream &lhs, Point &pt)
{
    if (pt.x < 300 || pt.y < 100 || pt.x > 1000 || pt.y > 700)
        lhs << "weird: ";
    lhs << "{x: " << pt.x << ", y: " << pt.y << "}";
    return lhs;
}

#define NB_POINTS 40

Vector2 randInCircle()
{
    float angle = fmod(rand(), 2*PI) + PI;
    float x = cosf(angle);
    float y = sinf(angle);
    return {x, y};
}

// int main(void)
// {
//     rngd::Generator generator;

//     generator.setup();

//     rlSetLineWidth(3);

//     while (!WindowShouldClose())
//     {
//         generator.update();
//         generator.draw();
//     }

//     generator.stop();
//     return 0;
// }
