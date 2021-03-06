#include "copsaiagent.h"

#include "astar.h"
#include "banditaiagent.h"
#include "oglwidget.h"

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

                if (next->agent != nullptr)
                {
                    if (next->type != DOOR && next->agent->get_type() == BANDIT)
                    {
                        capture(next->agent);
                        next->agent = nullptr;
                        OGLWidget::nCaptured++;

                        cout << "Agentes restantes: " << game->get_threads().size() << endl;
                    }
                }
            }
        }
    }
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
