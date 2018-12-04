#-------------------------------------------------
#
# Project created by QtCreator 2018-11-21T19:14:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtDogs
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        spritesheettool.cpp \
        dog.cpp \
        model.cpp

HEADERS += \
        mainwindow.h \
        include/test.h \
        spritesheettool.h \
        dog.h \
        model.h


FORMS += \
        mainwindow.ui

INCLUDEPATH += ../../Box2D-master
LIBS += -L"../../Box2D-master/Build/bin/x86_64/Debug"
LIBS += -lBox2D
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    assets/Untitled Document 1 \
    include/Untitled Document 1 \

$$PWD/../../SFML

LIBS += -L"../../SFML/lib"

CONFIG(release, debug|release): LIBS += -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network  -lsfml-system
CONFIG(debug, debug|release): LIBS +=  -lsfml-graphics-d -lsfml-window-d -lsfml-audio-d -lsfml-network-d -lsfml-system-d


INCLUDEPATH += "../../SFML/include"
DEPENDPATH += "../../SFML/include"
