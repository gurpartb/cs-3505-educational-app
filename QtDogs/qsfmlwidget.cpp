#include "qsfmlwidget.h"

QSFMLWidget::QSFMLWidget(QWidget* Parent) :
QWidget(Parent),
myInitialized(false)
{
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);

    // Set strong focus to enable keyboard events to be received
    setFocusPolicy(Qt::StrongFocus);

}

void QSFMLWidget::showEvent(QShowEvent *event)
{
    if(!myInitialized)
    {
        #ifdef Q_WS_X11
            XFlush(QX11Info::display());
        #endif

        // Create the SFML window with the widget handle
        //sf::RenderWindow::create(winId());

        // Let the derived class do its specific stuff
        OnInit();

        // Setup the timer to trigger a refresh at specified framerate
        connect(&myTimer, SIGNAL(timeout()), this, SLOT(repaint()));
        myTimer.start();

        myInitialized = true;
    }
}

QPaintEngine* QSFMLWidget::paintEngine() const
{
    return 0;
}

void QSFMLWidget::paintEvent(QPaintEvent*)
{
    //sf::RenderWindow::clear();
    // Let the derived class do its specific stuff
    OnUpdate();
    // Display on screen
   // sf::RenderWindow::display();
}

void QSFMLWidget::OnInit()
{
    // To be overridden
}
void QSFMLWidget::OnUpdate()
{
    // To be overridden
}
