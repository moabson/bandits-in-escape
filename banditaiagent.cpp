#include "banditaiagent.h"

void BanditAiAgent::run()
{
    cout << "run thread" << endl;
    for (Tile* next : path)
    {
        if (!game->is_equal(current, next))
        {
            this->sleep(1);
            next->agent = current->agent;
            current->agent = nullptr;
            current = next;

        }
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

