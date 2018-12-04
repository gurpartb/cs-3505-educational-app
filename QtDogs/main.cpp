#include "mainwindow.h"
#include <QApplication>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#define SCALE 10.f

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Model myModel;
    MainWindow w(&myModel);
    w.show();
    return a.exec();
}
