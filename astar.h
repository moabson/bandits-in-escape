#ifndef ASTAR_H
#define ASTAR_H

#include "game.h"
#include <vector>

using namespace std;

class AStar
{
public:
    AStar(Game* g) : game(g) {}
    vector<Tile*> find_path(Tile *start, Tile *target);
    int distance(Tile* current, Tile *target);
    Tile* get_tile_top(Tile *current);
    Tile* get_tile_prev(Tile *current);
    Tile* get_tile_next(Tile *current);
    Tile* get_tile_bottom(Tile *current);
    void reconstruct_path(Tile *current, vector<Tile*>* path);
    void compute(Tile* current, Tile* compute, Tile* target);
    bool open_add(Tile* tile);
    void closed_add(Tile* tile);

    bool in_closed(Tile* tile);
    void print_path(Tile *current);
    void clear_computedvalues();
    bool collision(Tile *check);

private:
    Game* game;
    priority_queue<Tile*, vector<Tile*>, comparator> open;
    vector<Tile*> closed;
    map<Tile*, Tile*> came_from;


};

#endif // ASTAR_H
