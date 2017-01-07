#-------------------------------------------------
#
# Project created by QtCreator 2017-01-07T23:18:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AirHockey
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

LIBS += -LE:\SFML\SFML-2.1-windows-gcc-4.7-mingw-32bits\SFML-2.1\lib

CONFIG(release, debug|release): LIBS += -lsfml-audio -lsfml-graphics -lsfml-main -lsfml-network -lsfml-window -lsfml-system
CONFIG(debug, debug|release): LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-main-d -lsfml-network-d -lsfml-window-d -lsfml-system-d

INCLUDEPATH += E:\SFML\SFML-2.1-windows-gcc-4.7-mingw-32bits\SFML-2.1\include
DEPENDPATH += E:\SFML\SFML-2.1-windows-gcc-4.7-mingw-32bits\SFML-2.1\include
