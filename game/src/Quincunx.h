
#pragma once
#include <Application.h>

#include <format>


#include "ErrorCodes.h"
#include "Logger.h"
#include "Components/TitleUpdater.h"


class Quincunx : public Penjin::Application {
    protected:
        void start() override {
            auto& go = scene().createGameObject("Title update");
            auto& test = go.addComponent<TitleUpdater>();

            LOG_DEBUG(std::format("Mesh init: {}", mesh_.load()));


            shader_ = Penjin::Shader::createUnlitColor();
            if (shader_ == nullptr) {
                LOG_ERROR(std::format("Unable to load shader!"));
                quit(Penjin::ErrorCode::ShaderLoadFailed);
            }
            shader_->setVec4("uColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        }
        void tick() override {
            Application::tick();
        }
        void draw() override {
            Application::draw();
            if (shader_ == nullptr) return;
            shader_->bind();
            mesh_.draw();
        }
    private:
        Penjin::Mesh mesh_;
        std::shared_ptr<Penjin::Shader> shader_;

};
