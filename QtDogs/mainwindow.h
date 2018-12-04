#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "spritesheettool.h"
#include <QTimer>
#include "model.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void update();

public:
    explicit MainWindow(Model* myModel,QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_trustProgressBar_valueChanged(int value);
    void on_hungerProgressBar_valueChanged(int value);
    void on_bathroomProgressBar_valueChanged(int value);

private:
    QTimer* timer;
    Ui::MainWindow *ui;
    SpriteSheetTool spriteSheetTool;

signals:
    void getDogAnimationSignal(std::string, int);

private slots:
    void playDogAnimation(QImage);

};

#endif // MAINWINDOW_H
