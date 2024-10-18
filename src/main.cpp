#include "ui/ui.h"

#include <qapplication.h>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    UI ui;
    ui.show();

    return QApplication::exec();
}