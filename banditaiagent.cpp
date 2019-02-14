#include "banditaiagent.h"
#include <QDebug>
#include <QMutex>
#include <QMutexLocker>

void BanditAiAgent::run()
{
//    qDebug() << "run thread";

    for (Tile* next : path)
    {
        this->msleep(150);

        if (!game->is_equal(current, next))
        {
//            qDebug() << "moving";

            if (next->agent == nullptr)
            {
                next->agent = current->agent;
                current->agent = nullptr;
                current = next;
            }
            else
            {
                qDebug() << "collision two agents";
                // tratar colisão de dois agentes
                // se for outro bandido fazer nada
                // se for policia morreu
            }
        }
//        mutex.unlock();
    }
}

void BanditAiAgent::set_current(Tile* current)
{
    this->current = current;
}

void BanditAiAgent::set_path(vector<Tile*> path)
{
    this->path = path;
}

BanditAiAgent::~BanditAiAgent()
{
}

