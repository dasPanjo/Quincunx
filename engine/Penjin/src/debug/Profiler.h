
#pragma once

namespace Penjin {
    class Profiler final{
    public:
        static void draw();
        static void endFrame();

        static unsigned long vertexCount;
        static unsigned long drawCalls;
    };
}
