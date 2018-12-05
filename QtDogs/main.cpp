#include "mainwindow.h"
#include <QApplication>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
