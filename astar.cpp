#include "astar.h"

// manhattan distance
int AStar::distance(Tile* current, Tile *target)
{
    int dx = abs(target->position.x - current->position.x);
    int dy = abs(target->position.y - current->position.y);

    return dx + dy;
}

/*
          top
           /\
            |
   prev <-- X --> next
            |
           \/
         bottom
*/

Tile* AStar::get_tile_top(Tile *current)
{
    int x = current->position.x;
    int y = current->position.y - 1;

    if (y < 0)
    {
        return nullptr;
    }

    return game->get_tile({.x = x, .y = y});
}

Tile* AStar::get_tile_bottom(Tile *current)
{
    int x = current->position.x;
    int y = current->position.y + 1;

    if (y >= game->get_y())
    {
        return nullptr;
    }

    return game->get_tile({.x = x, .y = y});
}

Tile* AStar::get_tile_next(Tile *current)
{
    int x = current->position.x + 1;
    int y = current->position.y;

    if (x >= game->get_x())
    {
        return nullptr;
    }

    return game->get_tile({.x = x, .y = y});
}

Tile* AStar::get_tile_prev(Tile *current)
{
    int x = current->position.x - 1;
    int y = current->position.y;

    if (x < 0)
    {
        return nullptr;
    }

    return game->get_tile({.x = x, .y = y});
}

void AStar::compute(Tile* current, Tile* compute, Tile* target)
{
    if (current != nullptr && compute != nullptr && target != nullptr)
    {
        compute->G = current->G + 1;
        compute->H = distance(compute, target);
        compute->F = compute->G + compute->H;
        compute->computed = true;
    }
}

bool AStar::open_add(Tile* tile)
{
    if (tile == nullptr && !tile->computed)
        return false;

    open.push(tile);

    return true;
}

void AStar::closed_add(Tile* tile)
{
    closed.push_back(tile);
}


bool AStar::in_closed(Tile* tile)
{
    for(Tile* current: closed)
    {
        if (game->is_equal(tile, current))
            return true;
    }

    return false;
}

bool AStar::collision(Tile *check)
{
    if (check->type == WALL)
        return true;

    return false;
}

void AStar::print_path(Tile *current)
{
    auto it = came_from.find(current);
    bool end = it == came_from.end();

    if (!end)
    {
        print_path(it->second);
        cout << it->second->position << endl;
    }
}

void AStar::clear_computedvalues()
{
    for(int y = 0; y < game->get_y(); ++y)
    {
        for(int x = 0; x <game->get_x(); ++x)
        {
            Tile *current = game->get_tile({.x = x, .y = y});

            current->F = 0;
            current->H = 0;
            current->G = 0;
        }
    }

    while(!open.empty()) open.pop();
    came_from.clear();
    closed.clear();
}

Tile* (AStar::*neighbors2[])(Tile*) = {
    &AStar::get_tile_top,
    &AStar::get_tile_prev,
    &AStar::get_tile_next,
    &AStar::get_tile_bottom
};

void AStar::reconstruct_path(Tile *current, vector<Tile*>* path)
{
    auto it = came_from.find(current);
    bool end = it == came_from.end();

    if (!end)
    {
        reconstruct_path(it->second, &(*path));
        cout << it->second->position << endl;
        path->push_back(it->second);
    }
}

// A-star based search
vector<Tile*> AStar::find_path(Tile *start, Tile *target)
{
    vector<Tile*> path;

    if (start == nullptr || target == nullptr)
        return path;

    if (!target->is_target) {
        return path;
    }

    if (start->is_target)
        return path;

    clear_computedvalues();

    open.push(start);

    while(!open.empty())
    {
        Tile* current = open.top();

        if (game->is_equal(current, target))
        {
//            print_path(current);
            reconstruct_path(current, &path);
            path.push_back(current);
            return path;
        }

        open.pop();
        closed_add(current);

        for(auto get_neighbor : neighbors2)
        {
            Tile* neighbor = (this->*get_neighbor)(current);

            if (neighbor != nullptr && !collision(neighbor))
            {
                if (!in_closed(neighbor))
                {
                    compute(current, neighbor, target);
                    open_add(neighbor);

                    if (current->G < neighbor->G)
                    {
                        came_from.insert(make_pair(neighbor, current));
                    }
                }
            }
        }
    }

    return path;
}
