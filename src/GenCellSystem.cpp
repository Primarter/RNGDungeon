#include "GenCellSystem.hpp"

namespace rngd
{
    bool GenCellSystem::aabbCollision(Rectangle a, Rectangle b) const
    {
        if (a.x < b.x + b.width &&
            a.x + a.width > b.x &&
            a.y < b.y + b.height &&
            a.height + a.y > b.y) {
            return true;
        }
        return false;
    }

    Vector2 GenCellSystem::cellCenter(const Rectangle &cell) const
    {
        return (Vector2Add({cell.x, cell.y}, Vector2Divide({cell.width, cell.height}, {2,2})));
    }

    void GenCellSystem::setup(size_t _seed UNUSED)
    {
        // if (_seed == GenCellSystem::RANDOM_SEED)
        //     this->seed = std::chrono::system_clock::now().time_since_epoch().count();
        // std::minstd_rand0 generator(seed);

        isMainRoom.fill(false);
        for (size_t i = 0; i < N; ++i) {
            this->cells[i] = {
                float(RANGE_RAND(-DIST_FROM_CENTER, DIST_FROM_CENTER)) + WIDTH / 2,
                float(RANGE_RAND(-DIST_FROM_CENTER, DIST_FROM_CENTER)) + HEIGHT / 2,
                float(RANGE_RAND(MIN_SIZE, MAX_SIZE)),
                float(RANGE_RAND(MIN_SIZE, MAX_SIZE))
            };
        }
    }

    void GenCellSystem::reset(size_t _seed)
    {
        setup(_seed);
        this->separated = false;
    }

    void GenCellSystem::separate(void)
    {
        this->separated = true;

        for (size_t i = 0; i < N; ++i) {
            Rectangle &rectA = cells[i];
            for (size_t j = 0; j < N; ++j) {
                if (i == j) continue;
                Rectangle &rectB = cells[j];
                if (aabbCollision(rectA, rectB)) {
                    this->separated = false;
                    Vector2 dir = Vector2Subtract(cellCenter(rectA), cellCenter(rectB));
                    dir = Vector2Normalize(dir);
                    dir = Vector2Multiply(dir, {GenCellSystem::STEERING_STRENGTH, GenCellSystem::STEERING_STRENGTH});

                    rectA.x += dir.x;
                    rectA.y += dir.y;
                    rectB.x -= dir.x;
                    rectB.y -= dir.y;
                }
            }
            // If optimisation is needed, uncomment this and comment the draw call
            // DrawRectangleRec(rectA, ORANGE);
            // DrawRectangleLinesEx(rectA, 1, BLACK);
        }
        if (this->separated)
            this->createGraph();
    }

    void GenCellSystem::createGraph(void)
    {
        float mean = 0.0;
        for (auto &&it : cells)
            mean += it.height*it.width;
        mean /= cells.size();
        this->graphIndices.clear();
        this->roomCenters.clear();

        for (size_t i = 0; i < N; i++) {
            if (cells[i].height * cells[i].width >= 1.25*mean) {
                isMainRoom[i] = true;
                this->roomCenters.push_back({
                    cells[i].x + cells[i].width/2, cells[i].y + cells[i].height/2
                });
            }
        }
        graphIndices = Triangulate(roomCenters);
        graphEdges = getEdges(roomCenters, graphIndices);
        graphEdges = Kruskal(roomCenters, graphEdges);
    }

    void GenCellSystem::updateGraph(void)
    {
    }

    void GenCellSystem::draw(void) const
    {
        for (size_t i = 0; i < N; ++i) {
            const Rectangle &r = this->cells[i];
            if (isMainRoom[i])
                DrawRectangleRec(r, RED);
            else
                DrawRectangleRec(r, DARKBLUE);
            DrawRectangleLinesEx(r, 1, BLACK);
        }
        if (this->separated) {
            // drawTrianglesFromIndices(this->roomCenters, this->graphIndices);
            drawEdgesFromIndices(roomCenters, graphEdges);
            for (auto &&pt : roomCenters) {
                DrawCircleV(pt, 5, GREEN);
            }
        }
    }

    std::array<Rectangle, N> GenCellSystem::getCells(void) const
    {
        return this->cells;
    }
} // namespace rngd