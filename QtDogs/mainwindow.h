#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void update();

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QTimer* timer;
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
