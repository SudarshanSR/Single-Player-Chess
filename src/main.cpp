#include "ui/window.h"

#include <qapplication.h>

std::int32_t main(std::int32_t argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return QApplication::exec();
}