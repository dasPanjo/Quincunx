
#pragma once
#include <Application.h>

#include <format>


#include "ErrorCodes.h"
#include "Logger.h"
#include "MeshRenderer.h"
#include "Model.h"
#include "Components/TitleUpdater.h"


class Quincunx : public Penjin::Application {
    protected:

        void start() override {
            auto gameManager = &scene().createGameObject("Title update");
            auto& test = gameManager->addComponent<TitleUpdater>();

            std::shared_ptr<Penjin::Shader> baseShader = Penjin::Shader::createUnlitColor();
            if (baseShader == nullptr) {
                return;
            }

            testGameObject1_ = &scene().createGameObject("Quad 1");
            auto& meshRenderer1 = testGameObject1_->addComponent<Penjin::MeshRenderer>();

            testGameObject2_ = &scene().createGameObject("Quad 2");
            auto& meshRenderer2 = testGameObject2_->addComponent<Penjin::MeshRenderer>();

            auto redMaterial = std::make_shared<Penjin::Material>(baseShader, "Red material");
            redMaterial->baseColor_ = {1.0f, 0.0f, 0.0f, 1.0f};

            auto greenMaterial = std::make_shared<Penjin::Material>(baseShader, "Green material");
            greenMaterial->baseColor_ = {0.0f, 1.0f, 0.0f, 1.0f};


            meshRenderer1.materials_.push_back(redMaterial);
            meshRenderer2.materials_.push_back(greenMaterial);

            std::shared_ptr<Penjin::Model> model = std::make_shared<Penjin::Model>();
            model->loadDemo();

            meshRenderer1.model_ = model;
            meshRenderer2.model_ = model;

        }
        void tick() override {
            Application::tick();
            if (testGameObject1_ == nullptr) return;
            testGameObject1_->transform().localPosition.x = std::sin(Penjin::Time::get().totalNanosecondsMs() * 0.001f) * 0.5f;
            testGameObject1_->transform().rotate(glm::vec3(Penjin::Time::get().deltaTime()));

            testGameObject2_->transform().localPosition.x = std::cos(Penjin::Time::get().totalNanosecondsMs() * 0.001f) * 0.5f;
            testGameObject2_->transform().rotate(glm::vec3(-Penjin::Time::get().deltaTime()));
        }
        void draw() override {
            Application::draw();
        }

    private:
        Penjin::GameObject* testGameObject1_ = nullptr;
        Penjin::GameObject* testGameObject2_ = nullptr;
};
