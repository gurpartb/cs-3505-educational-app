#ifndef QSFMLWIDGET_H
#define QSFMLWIDGET_H

#ifdef Q_WS_X11
    #include <Qt/qx11info_x11.h>
    #include <X11/Xlib.h>
#endif

#include <QObject>
#include <QWidget>
#include <QTimer>
#include <SFML/Graphics.hpp>
//#include <SFML/Graphics>

class QSFMLWidget : public QWidget, public sf::RenderWindow
{
    Q_OBJECT
public:
    QSFMLWidget(QWidget* Parent);

    virtual QPaintEngine *paintEngine() const;

private:
    virtual void OnInit();

    virtual void OnUpdate();

    virtual void showEvent(QShowEvent *event);

    virtual void paintEvent(QPaintEvent *event);

    QTimer myTimer;
    bool myInitialized;


signals:

public slots:
};

#endif // QSFMLWIDGET_H
