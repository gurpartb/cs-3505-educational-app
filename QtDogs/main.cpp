#include "mainwindow.h"
#include <QApplication>
#include "gravity.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    gravity g;
    w.show();
    return a.exec();
}
