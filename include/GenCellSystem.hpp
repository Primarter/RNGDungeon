#pragma once

#include "stdInclude.hpp"
#include "lib/Lib.hpp"
#include "raylib.h"

namespace rngd
{
    class GenCellSystem
    {
        private:
            std::array<Rectangle, N> cells;
            std::array<bool, N> isMainRoom;
            std::vector<Point> roomCenters;
            std::vector<ITriangle> graphIndices;
            std::vector<IEdge> graphEdges;
            size_t seed = 0;
        public:
            static const size_t RANDOM_SEED = 0;
            static constexpr float STEERING_STRENGTH = 1;
            bool separated = false;
            GenCellSystem(void) = default;
            ~GenCellSystem(void) = default;
            void setup(size_t seed);
            void reset(size_t seed);
            void separate(void);
            void createGraph(void);
            void updateGraph(void);
            std::array<Rectangle, N> getCells(void) const;
            bool aabbCollision(Rectangle a, Rectangle b) const;
            Vector2 cellCenter(const Rectangle &cell) const;
            void draw(void) const;
    };
} // namespace rngd
