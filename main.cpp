#include "joysticktest.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    joystickTest w;
    w.show();
    return a.exec();
}
