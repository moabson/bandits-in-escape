#include "oglwidget.h"
#include "texture.h"

using namespace std;

static int _WIDTH = 0;
static int _HEIGHT = 0;
static int _GRID_OFFSET_X = 0;
static int _GRID_OFFSET_Y = 0;

bool OGLWidget::_AIAGENT_OPTION = true;
AiAgentType OGLWidget::_AIAGENT_TYPE = COP;

bool OGLWidget::_TILE_OPTION = false;
TileType OGLWidget::_TILE_TYPE;

int OGLWidget::_FIELD = 1;

bool OGLWidget::_CLEAR_AIAGENTS = false;
bool OGLWidget::_CLEAR_FIELD = false;
bool OGLWidget::_LOAD_FIELD = false;

int OGLWidget::nCaptured = 0;
int OGLWidget::nEscaped = 0;

#define NMAX_X 16
#define NMAX_Y 12

Game* game = new Game(NMAX_X, NMAX_Y);
AStar astar(game);

OGLWidget::OGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1);
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

OGLWidget::~OGLWidget()
{
    delete game;
}


void OGLWidget::initializeGL()
{
    resizeGL(this->width(),this->height());

    Texture::loadTexture("floor.png", 0);
    Texture::loadTexture("wall.jpg", 1);
    Texture::loadTexture("door.png", 2);
    Texture::loadTexture("pmal.png", 3);
    Texture::loadTexture("bandit.png", 4);
    Texture::loadTexture("ufal.png", 5);
//    Texture::loadTexture("wood.jpg", 1);
}

void draw_tile(Tile* tile, int textureId)
{
    Position p = tile->position;

    if (p.x >= NMAX_X || p.y >= NMAX_Y)
        return;

    int minX, maxX;
    int minY, maxY;

    maxX = ((p.x == 0) ? 1 : p.x + 1) * _GRID_OFFSET_X;
    maxY = ((p.y == 0) ? 1 : p.y + 1) * _GRID_OFFSET_Y;

    minX = maxX - _GRID_OFFSET_X;
    minY = maxY - _GRID_OFFSET_Y;

    glBindTexture(GL_TEXTURE_2D, Texture::textures[textureId]);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 1.0f);
        glTexCoord2i(0, 0); glVertex2f(minX, minY); // top left
        glTexCoord2i(0, 1); glVertex2f(minX, maxY); // bottom left
        glTexCoord2i(1, 1); glVertex2f(maxX, maxY); // bottom right
        glTexCoord2i(1, 0); glVertex2f(maxX, minY); // top right
    glEnd();

    glDisable(GL_TEXTURE_2D);
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


void show_tiles()
{
    for (int y = 0; y < game->get_y(); ++y)
    {
        for (int x = 0; x < game->get_x(); ++x)
        {
            Tile* tile = game->get_tile({.x = x, .y = y});

            Color c;

            switch (tile->type) {
            case FLOOR:
                c = {.r = 0.5, .g = 0.3, .b = 0.9};
//                draw_grid_point({.x = x, .y = y}, c);
                draw_tile(tile, 0);
                break;
            case WALL:
//                c = {.r = 1.0, .g = 1.0, .b = 0.5};
                draw_tile(tile, 1);
                break;
            case DOOR:
//                c = {.r = 0.9, .g = 0.9, .b = 0.9};
//                draw_grid_point({.x = x, .y = y}, c);
                draw_tile(tile, 5);
                break;
            default:
                break;
            }

            if (tile->agent != nullptr)
            {
                switch (tile->agent->get_type()) {
                case BANDIT:
//                    draw_grid_point({.x = x, .y = y}, {.r = 1.0, .g = 0.0, .b = 0.0});
                    draw_tile(tile, 4);
                    break;

                case COP:
//                    draw_grid_point({.x = x, .y = y}, {.r = 0.0, .g = 0.0, .b = 1.0});
                    draw_tile(tile, 3);
                    break;
                default:
                    break;
                }
            }
        }
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
    Color c = {.r = 0.9, .g = 0.9, .b = 0.9};

    draw_grid_point({.x = 0, .y = 0}, c);

    if (_CLEAR_AIAGENTS)
    {
        game->clear_aiagents();
        _CLEAR_AIAGENTS = false;
    }
    else if (_CLEAR_FIELD)
    {
        game->clear_field();
        _CLEAR_FIELD = false;

        OGLWidget::nCaptured = 0;
        OGLWidget::nEscaped = 0;
    }
    else if (_LOAD_FIELD)
    {
        game->clear_aiagents();
        game->clear_field();

        string filename;

        switch (_FIELD) {
        case 1:
            filename = "mapa_cidade_universitaria";
            break;
        case 2:
            filename = "mapa_2";
            break;
        default:
            break;
        }

        game->loadmap(filename);

        _LOAD_FIELD = false;
    }
    else
    {
        show_tiles();

        for(AiAgent* aiagent : game->get_threads())
        {
            aiagent->start();
        }
    }

    glFlush();
}

void OGLWidget::mousePressEvent(QMouseEvent *event)
{
    int x = floor((float) event->x() / (_WIDTH / NMAX_X));
    int y = floor((float) event->y() / (_HEIGHT / NMAX_Y));

    Position p = {.x = x, .y = y};

    Tile *tile = game->get_tile(p);

    if (tile != nullptr)
    {
        if (tile->type != WALL && tile->type != DOOR)
        {
            if (_AIAGENT_OPTION && tile->agent == nullptr)
            {
                if (_AIAGENT_TYPE == COP)
                {
                    CopsAiAgent* cops = new CopsAiAgent(game);

                    Tile* start = game->get_tile({.x = p.x, .y = p.y});

                    start->agent = cops;

                    cops->set_current(game->get_tile({.x = p.x, .y = p.y}));

                    game->get_threads().push_back(cops);

                }
                else if (_AIAGENT_TYPE == BANDIT)
                {
                    BanditAiAgent* bandit = new BanditAiAgent(game);

                    Tile* start = game->get_tile({.x = p.x, .y = p.y});
                    Tile* target = game->get_tile({.x = 15, .y = 6});

                    start->agent = bandit;

                    bandit->set_current(game->get_tile({.x = p.x, .y = p.y}));
                    bandit->set_target(target);
//                    bandit->set_path(astar.find_path(start, target));

                    game->get_threads().push_back(bandit);
                }
            }
        }

        if (_TILE_OPTION && !_AIAGENT_OPTION && tile->agent == nullptr)
        {
            Tile t;
            t.position = p;
            t.type = _TILE_TYPE;

            game->add_tile(t);
        }
    }
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
