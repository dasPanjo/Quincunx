#pragma once
#include <format>

#include <Application.h>
#include <Component.h>
#include <imgui.h>
#include <Input.h>
#include <Time.h>

class TitleUpdater : public Penjin::Component{
    public:
    void tick() override {
        Component::tick();
        Penjin::Application::get().window().setTitle(std::format("Quincunx - {:0.3f}ms ({:3.0f}fps)", Penjin::Time::get().deltaTimeMs(), 1.0f / Penjin::Time::get().deltaTime()));

        if (Penjin::Input::get().isKeyPressed(Penjin::Q)) {
            Penjin::Application::get().quit();
        }
    }
};
