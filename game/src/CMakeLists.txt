add_executable(Quincunx
        Main.cpp
        Quincunx.h
        Components/TitleUpdater.h
)

target_link_libraries(Quincunx PRIVATE
        Penjin
)