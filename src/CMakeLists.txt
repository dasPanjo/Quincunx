add_executable(HelloSnake main.cpp)

target_link_libraries(HelloSnake PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    glad::glad
    glm::glm
)
