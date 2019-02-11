#include "game.h"

Game::Game(int MAX_X, int MAX_Y)
{
    this->MAX_X = MAX_X;
    this->MAX_Y = MAX_Y;

    this->field = new Tile*[MAX_Y];
    for (int y = 0; y < MAX_Y; ++y)
    {
        this->field[y] = new Tile[MAX_X];
    }

    for(int y = 0; y < MAX_Y; ++y)
    {
        for(int x = 0; x < MAX_X; ++x)
        {
            add_tile(Tile(FLOOR, {.x = x, .y = y}));
        }
    }

    Tile t1_VICTIM = Tile(VICTIM, {.x = 4, .y = 2});
    t1_VICTIM.is_target = true;

    Tile t2_VICTIM = Tile(VICTIM, {.x = 1, .y = 4});
    t2_VICTIM.is_target = true;

    add_tile(Tile(ZOMBIE, {.x = 0, .y = 2}));
    add_tile(Tile(WALL, {.x = 2, .y = 1}));
    add_tile(Tile(WALL, {.x = 2, .y = 2}));
    add_tile(Tile(WALL, {.x = 2, .y = 3}));
    add_tile(t1_VICTIM);
    add_tile(t2_VICTIM);
}

bool Game::add_tile(Tile tile)
{
    int x = tile.position.x;
    int y = tile.position.y;

    if (x >= 0 && y >=0 && x < MAX_X && y < MAX_Y)
    {
        field[y][x] = tile;
        return true;
    }

    return false;
}

Tile* Game::get_tile(Position pos)
{
    return &field[pos.y][pos.x];
}

// manhattan distance
int Game::distance(Tile* current, Tile *target)
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

Tile* Game::get_tile_top(Tile *current)
{
    int x = current->position.x;
    int y = current->position.y - 1;

    if (y < 0)
    {
        return nullptr;
    }

    return &field[y][x];
}

Tile* Game::get_tile_bottom(Tile *current)
{
    int x = current->position.x;
    int y = current->position.y + 1;

    if (y >= MAX_Y)
    {
        return nullptr;
    }

    return &field[y][x];
}

Tile* Game::get_tile_next(Tile *current)
{
    int x = current->position.x + 1;
    int y = current->position.y;

    if (x >= MAX_X)
    {
        return nullptr;
    }

    return &field[y][x];
}

Tile* Game::get_tile_prev(Tile *current)
{
    int x = current->position.x - 1;
    int y = current->position.y;

    if (x < 0)
    {
        return nullptr;
    }

    return &field[y][x];
}

void Game::compute(Tile* current, Tile* compute, Tile* target)
{
    if (current != nullptr && compute != nullptr && target != nullptr)
    {
        compute->G = current->G + 1;
        compute->H = distance(compute, target);
        compute->F = compute->G + compute->H;
        compute->computed = true;
    }
}

bool Game::open_add(Tile* tile)
{
    if (tile == nullptr && !tile->computed)
        return false;

    open.push(tile);

    return true;
}

bool Game::closed_add(Tile* tile)
{
    closed.push_back(tile);
}

bool Game::is_equal(Tile *a, Tile *b)
{
    return ((a->position.x == b->position.x)
        && (a->position.y == b->position.y));
}

bool Game::in_closed(Tile* tile)
{
    for(Tile* current: closed)
    {
        if (is_equal(tile, current))
            return true;
    }

    return false;
}

bool Game::collision(Tile *check)
{
    if (check->type == WALL)
        return true;

    return false;
}

void Game::print_path(Tile *current)
{
    auto it = came_from.find(current);
    bool end = it == came_from.end();

    if (!end)
    {
        print_path(it->second);
        cout << it->second->position << endl;
    }
}

void Game::clear_computedvalues()
{
    for(int y = 0; y < MAX_Y; ++y)
    {
        for(int x = 0; x < MAX_X; ++x)
        {
            Tile *current = &field[y][x];

            current->F = 0;
            current->H = 0;
            current->G = 0;
        }
    }

    while(!open.empty()) open.pop();
    came_from.clear();
    closed.clear();
}

Tile* (Game::*neighbors[])(Tile*) = {
    &Game::get_tile_top,
    &Game::get_tile_prev,
    &Game::get_tile_next,
    &Game::get_tile_bottom
};

// A-star based search
bool Game::search(Tile *start, Tile *target)
{
    if (start == nullptr || target == nullptr)
        return false;

    if (!target->is_target) {
        return false;
    }

    if (start->is_target)
        return false;

    clear_computedvalues();

    open.push(start);

    while(!open.empty())
    {
        Tile* current = open.top();

        if (is_equal(current, target))
        {
            print_path(current);
            return true;
        }

        open.pop();
        closed_add(current);

        for(auto get_neighbor : neighbors)
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

    return false;
}

void Game::print_field()
{
    for(int y = 0; y < MAX_Y; ++y)
    {
        for(int x = 0; x < MAX_X; ++x)
        {
            cout << "[" << field[y][x].position << " " << field[y][x].type << "] ";
        }
        cout << endl;
    }
}

Game::~Game()
{
    for (int y = 0; y < MAX_Y; ++y)
    {
        delete [] this->field[y];
    }

    delete [] this->field;
}
