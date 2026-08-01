# External Dependencies suchen
find_package(SDL2 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# Penjin-Library definieren
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
)

# Header-Pfade von Penjin nach außen sichtbar machen
target_include_directories(Penjin
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/core
        ${CMAKE_CURRENT_SOURCE_DIR}/window
)

# External Dependencies an Penjin hängen
target_link_libraries(Penjin
        PUBLIC
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
        glad::glad
        glm::glm
)