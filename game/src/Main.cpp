#include <Application.h>
#include <WindowSettings.h>

#include "Quincunx.h"

int main(int argc, char** argv) {
    Quincunx app;
    int result = app.run(Penjin::WindowSettings::fromArgs(argc, argv));
    return result;
}
