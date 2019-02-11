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


#define MAX_X 5
#define MAX_Y 5

enum TileType
{
    FLOOR,
    WALL,
    ZOMBIE,
    VICTIM
};

struct Color
{
    GLfloat r, g, b;
};

struct Position
{
    int x, y;
};

struct Tile
{
    TileType type = FLOOR;
    Position position = {.x = 0, . y = 0};
    int F = 0, G = 0, H = 0;
    bool computed = false;
    bool is_target = false;

    Tile() {}
    Tile(TileType t, Position p, int f, int g, int h) :
        type(t),
        position(p),
        F(f),
        G(g),
        H(h)
        {}
    Tile(TileType type) : type(type) {}
    Tile(TileType t, Position p) : type(t), position(p) {}
};

struct comparator
{
    bool operator()(const Tile* a, const Tile* b) const
    {
        return (b->F < a->F);
    }
};

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
    friend std::ostream& operator<< (std::ostream& cout, Position& pos)
    {
        cout << "(" << pos.x << "," << pos.y << ")";
        return cout;
    }

private:


};

#endif // OGLWIDGET_H
