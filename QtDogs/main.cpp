/**
  CS 3505 - A8 Final Project - QT Dogs
  Educational application to teach youth the importance of pet responsibility.
  Designed by:
  Brendan Johnston, Andrew Dron, Caleb Edwards, Colton Lee, Gurpartap Bhatti, Jacob Haydel, Tyler Trombley, Jared Hansen
*/

#include "mainwindow.h"
#include <QApplication>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

///
/// \brief main
/// Main method for the QT Dogs project, starts the application.
/// \return
///
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
