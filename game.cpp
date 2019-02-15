#include "game.h"

#include "banditaiagent.h"
#include "copsaiagent.h"

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

//    Tile t1_VICTIM = Tile(DOOR, {.x = 4, .y = 2});
//    t1_VICTIM.is_target = true;

    Tile t2_VICTIM = Tile(DOOR, {.x = 15, .y = 6});
    t2_VICTIM.is_target = true;

    Tile cops1 = Tile(FLOOR, {.x = 14, .y = 6});
    cops1.agent = new CopsAiAgent(this);
    threads.push_back(cops1.agent);

    Tile bandit = Tile(FLOOR, {.x = 0, .y = 2});
    bandit.agent = new BanditAiAgent(this);
    threads.push_back(bandit.agent);

    Tile bandit2 = Tile(FLOOR, {.x = 0, .y = 8});
    bandit2.agent = new BanditAiAgent(this);
    threads.push_back(bandit2.agent);

    add_tile(bandit);
    add_tile(bandit2);
    add_tile(cops1);
    add_tile(Tile(WALL, {.x = 2, .y = 1}));
    add_tile(Tile(WALL, {.x = 2, .y = 2}));
    add_tile(Tile(WALL, {.x = 2, .y = 3}));

    add_tile(Tile(WALL, {.x = 4, .y = 4}));
    add_tile(Tile(WALL, {.x = 4, .y = 5}));
    add_tile(Tile(WALL, {.x = 4, .y = 6}));
    add_tile(Tile(WALL, {.x = 4, .y = 7}));

    add_tile(Tile(WALL, {.x = 2, .y = 8}));
    add_tile(Tile(WALL, {.x = 2, .y = 9}));
    add_tile(Tile(WALL, {.x = 2, .y = 10}));

//    add_tile(t1_VICTIM);
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

bool Game::is_equal(Tile *a, Tile *b)
{
    return ((a->position.x == b->position.x)
        && (a->position.y == b->position.y));
}

void Game::print_field()
{
    for(int y = 0; y < MAX_Y; ++y)
    {
        for(int x = 0; x < MAX_X; ++x)
        {
            Tile* tile = &field[y][x];

            cout << "[" << tile->position << " " << tile->type << " ";

//            if (tile->agent)
//                cout << " x ";

            cout << "] ";
        }
        cout << endl;
    }
}

int Game::get_x()
{
    return MAX_X;
}

int Game::get_y()
{
    return MAX_Y;
}

Tile** Game::get_field()
{
    return field;
}

vector<AiAgent*>& Game::get_threads()
{
    return threads;
}

Tile* Game::get_tile(AiAgent *agent)
{
    auto it = aiagent_map.find(agent);
    bool end = it == aiagent_map.end();

    if (!end)
    {
        Tile* tile = it->second;
        return tile;
    }

    return nullptr;
}

void Game::kill(AiAgent *agent)
{
    if (!agent->isFinished())
        agent->terminate();

    threads.erase(remove(threads.begin(), threads.end(), agent));
}

Game::~Game()
{
    cout << "Limpando memória" << endl;

//    for (AiAgent* a : *get_threads())
//    {
//        a->terminate();
//    }

    for (int y = 0; y < MAX_Y; ++y)
    {
        for (int x = 0; x < MAX_X; ++x)
        {
            Tile* tile = &field[y][x];

            if (tile->agent != nullptr)
            {
                if (!tile->agent->isFinished())
                    tile->agent->terminate();

//                delete tile->agent;
            }
        }
    }

    for (int y = 0; y < MAX_Y; ++y)
    {
        delete [] this->field[y];
    }

    delete [] this->field;
}
