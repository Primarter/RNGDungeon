#include "stdInclude.hpp"
#include "GenCellSystem.hpp"

namespace rngd
{
    class Generator
    {
        private:
            GenCellSystem genCellSys;
            std::string fileName;
            void drawImGui(void);
            void drawGen(void);
        public:
            Generator(void);
            ~Generator(void) = default;
            void setup(void);
            void update(void);
            void draw(void);
            void reset(void);
            void stop(void);
            void exportToFile(void);
    };
} // namespace rngd
