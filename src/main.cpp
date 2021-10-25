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

int main(void)
{
    rngd::Generator generator;

    generator.setup();
    std::vector<Point> pts;

    for (int i = 0; i < NB_POINTS; i++) {
        Vector2 p = randInCircle();
        pts.push_back({
            Vector2Add(Vector2Multiply(p, {(float)(rand() % 300), (float)(rand() % 300)}), {WIDTH/2, HEIGHT/2})
        });
    }
    std::sort(pts.begin(), pts.end(), [](const Point &a, const Point &b) {return a.x < b.x;});

    // std::vector<Delaunay::ITriangle> triIndices = Triangulate(pts);

    while (!WindowShouldClose())
    {
        // BeginDrawing();
        // ClearBackground(BLACK);
        // drawTrianglesFromIndices(pts, triIndices);
        // EndDrawing();
        generator.update();
        generator.draw();
    }

    generator.stop();
    return 0;
}
