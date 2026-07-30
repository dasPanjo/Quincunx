add_executable(Quincunx
        Main.cpp
)

# Linkt Penjin UND zieht sich automatisch SDL2, glad, glm & die Include-Pfade mit!
target_link_libraries(Quincunx PRIVATE
        Penjin
)