
#include "Profiler.h"

#include <imgui.h>

#include "Time.h"

unsigned long Penjin::Profiler::vertexCount = 0;
unsigned long Penjin::Profiler::drawCalls = 0;

void Penjin::Profiler::draw() {
    ImGui::Begin("Profiler", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("FPS: %.0f", 1.0f / Penjin::Time::get().deltaTime());
    ImGui::Text("Vertecies: %lu", vertexCount);
    ImGui::Text("Drawcalls: %lu", drawCalls);
    ImGui::End();
}

void Penjin::Profiler::endFrame() {
    vertexCount = 0;
    drawCalls = 0;
}
