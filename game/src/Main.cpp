#include <Application.h>
#include <WindowSettings.h>

int main(int argc, char** argv) {
    Penjin::Application app;
    return app.run(Penjin::WindowSettings::fromArgs(argc, argv));
}
