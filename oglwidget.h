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

#include "game.h"
#include "astar.h"
#include "aiagent.h"
#include "banditaiagent.h"
#include "mythread.h"

#define MAX_X 5
#define MAX_Y 5

class OGLWidget : public QOpenGLWidget
{

public:
    OGLWidget(QWidget *parent = 0);
    ~OGLWidget();


protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent *event);

private:


};

#endif // OGLWIDGET_H
