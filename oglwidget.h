#ifndef OGLWIDGET_H
#define OGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QTimer>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glut.h>

#include <iostream>
#include <cmath>
#include <functional>
#include <vector>
#include <string.h>
#include <QThread>
#include <QDebug>
#include <QString>
#include <fstream>

#include "game.h"
#include "astar.h"
#include "aiagent.h"
#include "banditaiagent.h"
#include "copsaiagent.h"
#include "mythread.h"

#define MAX_X 5
#define MAX_Y 5

class OGLWidget : public QOpenGLWidget
{

public:
    OGLWidget(QWidget *parent = 0);
    ~OGLWidget();

    static bool _AIAGENT_OPTION;
    static bool _TILE_OPTION;

    static int _FIELD;
    static AiAgentType _AIAGENT_TYPE;
    static TileType _TILE_TYPE;

    static bool _CLEAR_AIAGENTS;
    static bool _CLEAR_FIELD;
    static bool _LOAD_FIELD;

    static int nCaptured;
    static int nEscaped;


protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent *event);


};

#endif // OGLWIDGET_H
