#include "stdInclude.hpp"
#include "CellSystem.hpp"

namespace rngd
{
    class Generator
    {
        private:
            CellSystem cells;
        public:
            Generator(void) = default;
            ~Generator(void) = default;
            void setup(void);
            void tick(void);
            void draw(void);
    };
} // namespace rngd
