#ifndef GAME_H
#define GAME_H

#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glut.h>

#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <cmath>
#include <map>

using namespace  std;

//#define MAX_X 5
//#define MAX_Y 5

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

inline ostream& operator<< (ostream& cout, Position& pos)
{
    cout << "(" << pos.x << "," << pos.y << ")";
    return cout;
}

class Game
{
public:
    Game(int MAX_X, int MAX_Y);
    ~Game();
    void init();
    int distance(Tile* current, Tile *target);
    bool add_tile(Tile tile);
    Tile* get_tile(Position pos);
    Tile* get_tile_top(Tile *current);
    Tile* get_tile_prev(Tile *current);
    Tile* get_tile_next(Tile *current);
    Tile* get_tile_bottom(Tile *current);
    void compute(Tile* current, Tile* compute, Tile* target);
    bool open_add(Tile* tile);
    bool closed_add(Tile* tile);
    bool is_equal(Tile *a, Tile *b);
    bool in_closed(Tile* tile);
    bool collision(Tile *check);
    void print_path(Tile *current);
    void clear_computedvalues();
    bool search(Tile *start, Tile *target);
    void print_field();

private:
    Tile **field;
    priority_queue<Tile*, vector<Tile*>, comparator> open;
    vector<Tile*> closed;
    map<Tile*, Tile*> came_from;
    int MAX_X, MAX_Y;

};

#endif // GAME_H
