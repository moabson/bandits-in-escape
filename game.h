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

enum TileType
{
    FLOOR,
    WALL,
    ZOMBIE,
    VICTIM,
    DOOR
};

enum Movment
{
    TOP,
    LEFT,
    RIGHT,
    BOTTOM
};

struct Color
{
    GLfloat r, g, b;
};

struct Position
{
    int x, y;
};

typedef class AiAgent AiAgent;


// Map Agent ~> Tile

struct Tile
{
    AiAgent* agent = nullptr;
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

typedef struct Color Color;
typedef struct Position Position;
typedef struct Tile Tile;
typedef class AiAgent AiAgent;

class Game
{
public:
    Game(int MAX_X, int MAX_Y);
    ~Game();
    void init();
    void print_field();
    bool add_tile(Tile tile);
    bool move_tile(Tile *current, Movment movment);
    bool is_equal(Tile *a, Tile *b);
    int get_x();
    int get_y();
    Tile* get_tile(Position pos);
    Tile** get_field();
    vector<AiAgent*>* get_threads();

private:
    Tile **field;
    map<AiAgent*, Tile*> aiagent_map;
    int MAX_X, MAX_Y;
    vector<AiAgent*> threads;

};

#endif // GAME_H
