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
                float(RANGE_RAND(MIN_SIZE, MAX_SIZE)/GRIDRES)*GRIDRES,
                float(RANGE_RAND(MIN_SIZE, MAX_SIZE)/GRIDRES)*GRIDRES
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
        if (this->separated) {
            this->roundPositions();
            this->createGraph();
        }
    }

    void GenCellSystem::roundPositions(void) {
        for (auto &&cell : this->cells) {
            cell.x = round(cell.x/GRIDRES)*GRIDRES;
            cell.y = round(cell.y/GRIDRES)*GRIDRES;
        }
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
        auto totalEdges = getEdges(roomCenters, graphIndices);
        graphEdges = Kruskal(roomCenters, totalEdges);
        corridors = getCorridors(getEdgeVal(roomCenters, graphEdges));
    }

    void GenCellSystem::updateGraph(void)
    {
    }

    void GenCellSystem::draw(void) const
    {
        for (size_t i = 0; i < WIDTH; i += GRIDRES) {
            DrawLine(i, 0, i, HEIGHT, {120, 120, 120, 120});
            DrawLine(0, i, WIDTH, i, {120, 120, 120, 120});
        }
        for (size_t i = 0; i < N; ++i) {
            const Rectangle &r = this->cells[i];
            if (isMainRoom[i])
                DrawRectangleRec(r, RED);
            else
                DrawRectangleRec(r, DARKBLUE);
            DrawRectangleLinesEx(r, 1, BLACK);
        }
        if (this->separated) {
            drawEdgesFromIndices(roomCenters, graphEdges);
            for (auto &&pt : roomCenters) {
                DrawCircleV(pt, 5, PURPLE);
            }
            drawCorridors(corridors);
        }
    }

    std::array<Rectangle, N> GenCellSystem::getCells(void) const
    {
        return this->cells;
    }

    Grid GenCellSystem::exportGrid(void) const
    {
        int minX = INT32_MAX, minY = INT32_MAX, maxX = INT32_MIN, maxY = INT32_MIN;
        for (auto &&room : this->cells) {
            // std::cout << room.x << ", " << room.y << std::endl;
            if (room.x < minX) minX = room.x;
            if (room.y < minY) minY = room.y;
            if (room.x + room.width > maxX) maxX = room.x + room.width;
            if (room.y + room.height > maxY) maxY = room.y + room.height;
        }
        // std::cout << "minX: " << minX << ", minY: " << minY << ", maxX: " << maxX << ", maxY: " << maxY << std::endl << std::endl;
        auto gridWidth = (maxX - minX)/GRIDRES;
        auto gridHeight = (maxY - minY)/GRIDRES;
        std::cout << "gridWidth: " << gridWidth << std::endl;
        std::cout << "gridHeight: " << gridHeight << std::endl;
        std::vector<std::vector<Cell>> grid(gridHeight, std::vector<Cell>(gridWidth, {EMPTY, 0}));
        int idx = 0;
        for (auto &&room : this->cells) {
            if (!isMainRoom[idx++])
                continue;
            int startX = (room.x - minX)/GRIDRES;
            int startY = (room.y - minY)/GRIDRES;
            std::cout << "startX:" << startX << std::endl;
            std::cout << "startY:" << startY << std::endl;
            for (int y = 0; y < room.height / GRIDRES; y += 1) {
                for (int x = 0; x < room.width / GRIDRES; x += 1) {
                    grid[y + startY][x + startX] = (Cell){ROOM, idx};
                }
            }
        }
        // idx = 0;
        // for (auto &&corr : this->corridors) {
        //     for (int x = corr.p1.x - minX; x < corr.p2.x - minX; x += GRIDRES) {
        //         for (int y = corr.p1.y - ((THICKNESS*GRIDRES)/2); y < corr.p1.y + ((THICKNESS*GRIDRES)/2); y += GRIDRES) {
        //             int gridIdx = (gridWidth*(y/GRIDRES) + (x/GRIDRES));
        //             if (grid[gridIdx].type != ROOM) grid[gridIdx] = (Cell){CORRIDOR, idx};
        //         }
        //     }
        //     for (int y = corr.p2.x - minX; y < corr.p3.x - minX; y += GRIDRES) {
        //         for (int x = corr.p2.y - ((THICKNESS*GRIDRES)/2); x < corr.p2.y + ((THICKNESS*GRIDRES)/2); x += GRIDRES) {
        //             int gridIdx = (gridWidth*(y/GRIDRES) + (x/GRIDRES));
        //             if (grid[gridIdx].type != ROOM) grid[gridIdx] = (Cell){CORRIDOR, idx};
        //         }
        //     }
        //     idx++;
        // }

        return {grid, gridWidth, gridHeight};
    }
} // namespace rngd