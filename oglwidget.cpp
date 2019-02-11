#include "oglwidget.h"

using namespace std;

OGLWidget::OGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);

    cout << "Construtor" << endl;
}

OGLWidget::~OGLWidget(){

}


void OGLWidget::initializeGL()
{
    resizeGL(this->width(),this->height());
}

void OGLWidget::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(1.0, 1.0, 0.0);
    glRecti(300, 300, 790, 10);

    glColor3f(1.0, 0.0, 0.0);
    glRecti(300, 300, 790, 310);

    glFlush();

}

void OGLWidget::mousePressEvent(QMouseEvent *event)
{

}

void OGLWidget::resizeGL(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);

    glOrtho(0.0f, w, h, 0.0f, 0.0f, 1.0f);
}
