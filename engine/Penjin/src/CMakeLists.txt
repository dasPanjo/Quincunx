# External Dependencies suchen
find_package(SDL3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

add_library(Penjin
        core/Application.cpp
        core/Application.h
        time/Time.cpp
        time/Time.h
        window/Window.cpp
        window/Window.h
        window/WindowSettings.cpp
        window/WindowSettings.h
        logger/LogLevel.h
        logger/ILogSink.h
        logger/ConsoleSink.h
        logger/FileSink.cpp
        logger/FileSink.h
        logger/Logger.h
        logger/Logger.cpp
        input/Input.cpp
        input/Input.h
        input/KeyCode.h
        input/MouseButton.h
)

target_include_directories(Penjin
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/core
        ${CMAKE_CURRENT_SOURCE_DIR}/window
)

target_link_libraries(Penjin
        PUBLIC
        $<IF:$<TARGET_EXISTS:SDL3::SDL3>,SDL3::SDL3,SDL3::SDL3-static>
        glad::glad
        glm::glm
        imgui::imgui
)