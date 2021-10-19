#include "stdInclude.hpp"
#include "GenCellSystem.hpp"

namespace rngd
{
    class Generator
    {
        private:
            GenCellSystem genCellSys;
            void drawImGui(void);
            void drawGen(void);
        public:
            Generator(void) = default;
            ~Generator(void) = default;
            void setup(void);
            void update(void);
            void draw(void);
            void reset(void);
            void stop(void);
    };
} // namespace rngd
