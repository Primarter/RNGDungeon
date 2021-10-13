#include "CellSystem.hpp"

namespace rngd
{
    bool CellSystem::aabbCollision(Rectangle a, Rectangle b) const
    {
        if (a.x < b.x + b.width &&
            a.x + a.width > b.x &&
            a.y < b.y + b.height &&
            a.height + a.y > b.y) {
            return true;
        }
        return false;
    }

    Vector2 CellSystem::cellCenter(const Rectangle &cell) const
    {
        return (Vector2Add({cell.x, cell.y}, Vector2Divide({cell.width, cell.height}, {2,2})));
    }

    void CellSystem::setup(size_t _seed)
    {
        sw.start();
        // if (_seed == CellSystem::RANDOM_SEED)
        //     this->seed = std::chrono::system_clock::now().time_since_epoch().count();
        // std::minstd_rand0 generator(seed);

        for (size_t i = 0; i < N; ++i) {
            this->cells[i] = {
                float(RANGE_RAND(rand(), -DIST_FROM_CENTER, DIST_FROM_CENTER)) + WIDTH / 2,
                float(RANGE_RAND(rand(), -DIST_FROM_CENTER, DIST_FROM_CENTER)) + HEIGHT / 2,
                float(RANGE_RAND(rand(), MIN_SIZE, MAX_SIZE)),
                float(RANGE_RAND(rand(), MIN_SIZE, MAX_SIZE))
            };
        }
    }

    void CellSystem::tick(void)
    {
        for (size_t i = 0; i < N; ++i) {
            Rectangle &rectA = cells[i];
            for (size_t j = 0; j < N; ++j) {
                if (i == j) continue;
                Rectangle &rectB = cells[j];
                if (aabbCollision(rectA, rectB)) {
                    Vector2 dir = Vector2Subtract(cellCenter(rectA), cellCenter(rectB));
                    dir = Vector2Normalize(dir);
                    dir = Vector2Multiply(dir, {CellSystem::STEERING_STRENGTH, CellSystem::STEERING_STRENGTH});

                    rectA.x += dir.x;
                    rectA.y += dir.y;
                    rectB.x -= dir.x;
                    rectB.y -= dir.y;
                }
            }
            // DrawRectangleRec(rectA, ORANGE);
            // DrawRectangleLinesEx(rectA, 1, BLACK);
        }
    }

    void CellSystem::draw(void) const
    {
        for (size_t i = 0; i < N; ++i) {
            const Rectangle &r = this->cells[i];
            DrawRectangleRec(r, ORANGE);
            DrawRectangleLinesEx(r, 1, BLACK);
        }
    }

    std::array<Rectangle, N> CellSystem::getCells(void) const
    {
        return this->cells;
    }
} // namespace rngd