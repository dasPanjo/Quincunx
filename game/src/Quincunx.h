
#pragma once
#include <Application.h>

#include <format>


#include "ErrorCodes.h"
#include "Logger.h"
#include "MeshRenderer.h"
#include "Model.h"
#include "../../engine/Penjin/src/debug/Gizmos.h"
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

            auto cameraGo = &scene().createGameObject("Main camera");
            auto& mainCamera = cameraGo->addComponent<Penjin::Camera>();
            scene().mainCamera = &mainCamera;

            testGameObject1_ = &scene().createGameObject("Quad 1");
            auto& meshRenderer1 = testGameObject1_->addComponent<Penjin::MeshRenderer>();
            testGameObject1_->transform().localPosition = {0.0f, 0.0f, 0.5f};

            testGameObject2_ = &scene().createGameObject("Quad 2");
            auto& meshRenderer2 = testGameObject2_->addComponent<Penjin::MeshRenderer>();
            testGameObject2_->transform().localPosition = {0.0f, 0.0f, -0.5f};

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

            scene().mainCamera->transform().localPosition = {-5.0f, 0.0f, -10.0f};

        }
        void tick() override {
            Application::tick();
            auto x = std::sin(Penjin::Time::get().totalNanosecondsMs() * 0.001f);
            auto z = std::cos(Penjin::Time::get().totalNanosecondsMs() * 0.001f);
            scene().mainCamera->transform().localPosition = {x * 7.0f, 2.0f, z * 7.0f};
            scene().mainCamera->transform().lookAt({0.0f, 0.0f, 0.0f});

        }
        void draw() override {
            Application::draw();
            Penjin::Gizmos::drawGrid();
        }

    private:
        Penjin::GameObject* testGameObject1_ = nullptr;
        Penjin::GameObject* testGameObject2_ = nullptr;
};
