#include "banditaiagent.h"
#include <QDebug>
#include <QMutex>
#include <QMutexLocker>

vector<Tile*>& BanditAiAgent::lock_target()
{
    if (current == nullptr || target == nullptr)
        return path;

    Tile* minTarget = nullptr;

    for (int y = 0; y < game->get_y(); ++y)
    {
        for (int x = 0; x < game->get_x(); ++x)
        {
            Tile* tile = game->get_tile({.x = x, .y = y});

            if (tile->type == DOOR)
            {
                if (minTarget == nullptr)
                {
                    minTarget = tile;
                }
                else
                {
                    int dist = astar.distance(current, tile);
                    int minDist = astar.distance(current, minTarget);

                    if (dist < minDist)
                    {
                        minTarget = tile;
                    }
                }
            }
        }
    }

    if (minTarget != nullptr)
    {
        path = astar.find_path(current, minTarget);
    }

    return path;
}

void BanditAiAgent::run()
{
    for (Tile* next : lock_target())
    {
        this->msleep(350);

        if (!game->is_equal(current, next))
        {
//            qDebug() << "moving";

            if (next->agent == nullptr)
            {
                next->agent = current->agent;
                current->agent = nullptr;
                current = next;

                if (next->type == DOOR)
                {
                    path.clear();
                    game->get_threads().erase(remove(game->get_threads().begin(), game->get_threads().end(), get_current()->agent));
                    get_current()->agent = nullptr;
                }
            }
            else
            {
                qDebug() << "collision two agents";
                this->msleep(350);
            }
        }
    }
}

Tile* BanditAiAgent::get_current()
{
    return current;
}

void BanditAiAgent::set_current(Tile* current)
{
    this->current = current;
}

void BanditAiAgent::set_target(Tile* target)
{
    this->target = target;
}

void BanditAiAgent::set_path(vector<Tile*> path)
{
    this->path = path;
}

BanditAiAgent::~BanditAiAgent()
{
}

