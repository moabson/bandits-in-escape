#-------------------------------------------------
#
# Project created by QtCreator 2019-02-11T00:19:35
#
#-------------------------------------------------

QT       += core gui opengl
LIBS     += -lGLU -lGL -lglut -lsfml-graphics -lsfml-window -lsfml-system

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bandits-in-escape
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    oglwidget.cpp \
    game.cpp \
    aiagent.cpp \
    banditaiagent.cpp \
    astar.cpp \
    mythread.cpp \
    copsaiagent.cpp \
    texture.cpp

HEADERS  += mainwindow.h \
    oglwidget.h \
    game.h \
    aiagent.h \
    banditaiagent.h \
    astar.h \
    mythread.h \
    copsaiagent.h \
    texture.h

FORMS    += mainwindow.ui

DISTFILES += \
    data/mapa_2.txt \
    data/mapa_cidade_universitaria.txt \
    data/textures/floor.jpg \
    data/textures/wall.jpg \
    data/textures/bandit.png \
    data/textures/door.png \
    data/textures/floor.png \
    data/textures/pmal.png \
    data/textures/ufal.png
