#include "loginwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginWindow w;
    w.show();
    return a.exec();

    // QT_ASSUME_STDERR_HAS_CONSOLE=1 в linux для работы qdebug
}
