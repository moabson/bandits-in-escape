#include "oglwidget.h"

using namespace std;

static int _WIDTH = 0;
static int _HEIGHT = 0;
static int _GRID_OFFSET_X = 0;
static int _GRID_OFFSET_Y = 0;

#define NMAX_X 16
#define NMAX_Y 12

Game game(NMAX_X, NMAX_Y);

static int pi = 0;
vector<Tile*> path2;
static int maxpi = 10;

OGLWidget::OGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(300);

    cout << "Construtor" << endl;

    game.print_field();

    Tile *K = game.get_tile({.x = 1, .y = 2});
    Tile *target1 = game.get_tile({.x = 4, .y = 2});
    Tile *target2 = game.get_tile({.x = 15, .y = 6});
    // Tile *target2 = target_with_mindist(K);

    cout << game.search(K, target1).size() << endl;

    for(Tile* t : game.search(K, target2))
    {
        path2.push_back(t);
    }


//    path2 = &game.search(K, target2);

}

void draw_point(Position p, Color c)
{
    glPointSize(2);

    glBegin(GL_POINTS);
       glColor3f(c.r, c.g, c.b);
       glVertex2i(p.x, p.y);
    glEnd();
}

void draw_point(Position p, GLfloat size, Color c)
{
    glPointSize(size);

    glBegin(GL_POINTS);
       glColor3f(c.r, c.g, c.b);
       glVertex2i(p.x, p.y);
    glEnd();
}

void draw_breseham_line_low(Position p1, Position p2, Color c)
{
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    int yi = 1;

    if (dy < 0)
    {
        yi = -1;
        dy = -dy;
    }

    int D = 2 * dy - dx;
    int y = p1.y;

    for (int x = p1.x; x <= p2.x; ++x)
    {
        draw_point({. x = x, .y = y}, c);

        if (D > 0)
        {
            y = y + yi;
            D = D - 2 * dx;
        }

        D = D + 2 * dy;
    }
}

void draw_breseham_line_high(Position p1, Position p2, Color c)
{
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    int xi = 1;

    if (dx < 0)
    {
        xi = -1;
        dx = -dx;
    }

    int D = 2 * dx - dy;
    int x = p1.x;

    for (int y = p1.y; y <= p2.y; ++y)
    {
         draw_point({. x = x, .y = y}, c);

         if (D > 0)
         {
            x = x + xi;
            D = D - 2 * dy;
         }

         D = D + 2 * dx;
    }
}

// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

void draw_bresenham_line(Position p1, Position p2, Color c)
{
    if (abs(p2.y - p1.y) < abs(p2.x - p1.x))
    {
        if (p1.x > p2.x)
        {
            draw_breseham_line_low(p2, p1, c);
        }
        else
        {
            draw_breseham_line_low(p1, p2, c);
        }
    }
    else
    {
        if (p1.y > p2.y)
        {
            draw_breseham_line_high(p2, p1, c);
        }
        else
        {
            draw_breseham_line_high(p1, p2, c);
        }
    }

}

OGLWidget::~OGLWidget(){

}


void OGLWidget::initializeGL()
{
    resizeGL(this->width(),this->height());
}

int i = 0;
int maxi = 280;

void animation_sample()
{
    if (i < maxi)
    {
        i = i + 10;
    }
    else
    {
        i = 0;
    }

    glColor3f(1.0, 1.0, 0.0);
    glRecti(300, 300, 790, 10);

    glColor3f(1.0, 0.0, 0.0);
    glRecti(300, 300, 790, 310 + i);
}


void draw_grid_point(Position p, Color c)
{
    if (p.x >= NMAX_X || p.y >= NMAX_Y)
        return;

    int minX, maxX;
    int minY, maxY;

    maxX = ((p.x == 0) ? 1 : p.x + 1) * _GRID_OFFSET_X;
    maxY = ((p.y == 0) ? 1 : p.y + 1) * _GRID_OFFSET_Y;

    minX = maxX - _GRID_OFFSET_X;
    minY = maxY - _GRID_OFFSET_Y;

    glColor3f(c.r, c.g, c.b);
    glRecti(minX, minY, maxX, maxY);
}

void animation_sample3()
{
    if (pi < path2.size())
    {
        Tile* current = path2.at(pi);

        draw_grid_point({.x = current->position.x, .y = current->position.y}, {.r = 0.6, .g = 0.1, .b = 0.5});

//        cout << i << endl;
        ++pi;
    }
    else
    {
        pi = 0;
    }
}


void show_tiles()
{
    for (int y = 0; y < game.get_y(); ++y)
    {
        for (int x = 0; x < game.get_x(); ++x)
        {
            Tile* tile = game.get_tile({.x = x, .y = y});

            Color c;

            switch (tile->type) {
            case FLOOR:
                c = {.r = 0.5, .g = 0.3, .b = 0.9};
                break;
            case WALL:
                c = {.r = 1.0, .g = 1.0, .b = 0.5};
                break;
            case ZOMBIE:
                c = {.r = 1.0, .g = 0.0, .b = 0.0};
                break;
            case VICTIM:
                c = {.r = 0.0, .g = 0.0, .b = 1.0};
                break;
            default:
                break;
            }

            draw_grid_point({.x = x, .y = y}, c);
        }
    }
}

int k = 0;
int maxK = 9;

void animation_sample2()
{
    if (k <= maxK)
    {
        draw_grid_point({.x = 7, .y = 9 - k}, {.r = 1.0, .g = 0.0, .b = 0.0});
        k++;
    }
    else
    {
        k = 0;
    }
}

void show_grid_lines(Color c)
{
    for (int y = 0; y < _HEIGHT; y += _GRID_OFFSET_Y)
    {
        for (int x = 0; x < _WIDTH; x += _GRID_OFFSET_X)
        {
            if (x >= _GRID_OFFSET_X)
            {
                draw_bresenham_line({.x = x, .y = 0}, {.x = x, .y = _HEIGHT}, c);
            }
        }

        if (y >= _GRID_OFFSET_Y)
        {
            draw_bresenham_line({.x = 0, .y = y}, {.x = _WIDTH, .y = y}, c);
        }
    }
}

void OGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    show_grid_lines({.r = 1.0, .g = 0.0, .b = 0.0});

    show_tiles();
    animation_sample3();

//    draw_grid_point({.x = 0, .y = 0}, {.r = 0.0, .g = 0.0, .b = 1.0});

//    animation_sample2();

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

    _WIDTH = w;
    _HEIGHT = h;

    // 12 x 16
    _GRID_OFFSET_X = _WIDTH / NMAX_X;
    _GRID_OFFSET_Y = _HEIGHT / NMAX_Y;
}
