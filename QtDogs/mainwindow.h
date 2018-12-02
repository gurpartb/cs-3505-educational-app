#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "spritesheettool.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    SpriteSheetTool spriteSheetTool;
signals:
    void getDogAnimationSignal(std::string, int);
private slots:
    void playDogAnimation(QImage);
};

#endif // MAINWINDOW_H
