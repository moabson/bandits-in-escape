#include "copsaiagent.h"

#include "astar.h"
#include "banditaiagent.h"

Tile* CopsAiAgent::lock_target()
{
    Tile* min = nullptr;

    for(AiAgent* agent : game->get_threads())
    {
        if (agent->get_type() == BANDIT)
        {
            BanditAiAgent* bandit = static_cast<BanditAiAgent*>(agent);
            Tile* bandit_tile = bandit->get_current();

            if (min == nullptr)
            {
                min = bandit_tile;
            }
            else
            {
                int dist = astar.distance(current, bandit_tile);
                int minDist = astar.distance(current, min);

                if (dist < minDist)
                {
                    min = bandit_tile;
                }
            }
        }
    }

    return min;
}

void CopsAiAgent::capture(AiAgent* agent)
{
    game->kill(agent);
}

void CopsAiAgent::run()
{
    Tile* target = lock_target();
    vector<Tile*> path = astar.find_path(current, target);

    if (!path.empty() && path.size() > 1)
    {
        Tile* next = path.at(1);

        this->msleep(350);

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
                qDebug() << "collision two agents BANDIT";
                capture(next->agent);

//                cout << next->agent << endl;

                // tratar colisão de dois agentes
                // se for outro bandido fazer nada
                // se for policia morreu
            }
        }
    }
    else if (path.size() == 1)
    {
        cout << "um " << endl;
    }

//    for (Tile* next : astar.find_path(current, target))
//    {
//        this->msleep(150);

//        if (!game->is_equal(current, next))
//        {
////            qDebug() << "moving";

//            if (next->agent == nullptr)
//            {
//                next->agent = current->agent;
//                current->agent = nullptr;
//                current = next;
//            }
//            else
//            {
//                qDebug() << "collision two agents";
//                // tratar colisão de dois agentes
//                // se for outro bandido fazer nada
//                // se for policia morreu
//            }
//        }
////        mutex.unlock();
//    }
}


Tile* CopsAiAgent::get_current()
{
    return current;
}

void CopsAiAgent::set_current(Tile *current)
{
    this->current = current;
}


void CopsAiAgent::set_target(Tile* target)
{
    this->target = target;
}

CopsAiAgent::~CopsAiAgent()
{

}
