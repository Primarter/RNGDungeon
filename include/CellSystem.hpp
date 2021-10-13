#pragma once

#include "stdInclude.hpp"
#include "lib/Lib.hpp"
#include "raylib.h"

namespace rngd
{
    class CellSystem
    {
        private:
            std::array<Rectangle, N> cells;
            size_t seed = 0;
            lib::Stopwatch sw;
        public:
            static const size_t RANDOM_SEED = 0;
            static constexpr float STEERING_STRENGTH = 1;
            CellSystem(void) = default;
            ~CellSystem(void) = default;
            void setup(size_t seed);
            void tick(void);
            std::array<Rectangle, N> getCells(void) const;
            bool aabbCollision(Rectangle a, Rectangle b) const;
            Vector2 cellCenter(const Rectangle &cell) const;
            void draw(void) const;
    };
} // namespace rngd
