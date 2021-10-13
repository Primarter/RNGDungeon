#include "stdInclude.hpp"
#include "lib/Lib.hpp"

#define N 40
#define DIST_FROM_CENTER 40
#define MIN_SIZE 20
#define MAX_SIZE 80

#define WIDTH 1280
#define HEIGHT 720

bool aabb_collision(Rectangle a, Rectangle b) {
  if (a.x < b.x + b.width &&
      a.x + a.width > b.x &&
      a.y < b.y + b.height &&
      a.height + a.y > b.y) {
    return true;
  }
  return false;
}

void setup(std::array<Rectangle, N> &cells)
{
    InitWindow(WIDTH, HEIGHT, "RNGDungeon");
    SetTargetFPS(60);
    rlImGuiSetup(true);
}

void draw(void)
{
BeginDrawing();
rlImGuiBegin();

    ClearBackground(DARKGRAY);

    bool open = true;
    ImGui::ShowDemoWindow(&open);

rlImGuiEnd();
EndDrawing();
}

int main(int argc, char const *argv[])
{
    std::cout << "Hello World!" << std::endl;
    std::array<Rectangle, N> cells;

    setup(cells);

    while (!WindowShouldClose())
    {
        draw();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}
