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

    loadmap("mapa_cidade_universitaria");

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

void Game::clear_aiagents()
{
    for (int y = 0; y < MAX_Y; ++y)
    {
        for (int x = 0; x < MAX_X; ++x)
        {
            Tile* tile = &field[y][x];

            if (tile->agent != nullptr)
            {
                kill(tile->agent);
                tile->agent = nullptr;
            }
        }
    }

    threads.clear();
}

void Game::clear_field()
{
    clear_aiagents();

    for(int y = 0; y < MAX_Y; ++y)
    {
        for(int x = 0; x < MAX_X; ++x)
        {
            add_tile(Tile(FLOOR, {.x = x, .y = y}));

        }
    }

}

void Game::loadmap(string filename)
{
    ifstream file("data/" + filename + ".txt");

    if (!file.is_open())
        return;

//    clear_aiagents();
    clear_field();

    for (int y = 0; y < MAX_Y; ++y)
    {
        for (int x = 0; x < MAX_X; ++x)
        {
            int type;

            file >> type;

            TileType tiletype = static_cast<TileType>(type);

            add_tile(Tile(tiletype, {.x = x, .y = y}));
        }
    }

    file.close();
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
