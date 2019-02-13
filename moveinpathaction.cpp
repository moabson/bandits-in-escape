#include "moveinpathaction.h"

void MoveInPathAction::execute()
{
    for (Tile* next : path)
    {
        if (!game->is_equal(current, next))
        {
            next->agent = current->agent;
            current->agent = nullptr;
            current = next;
          QThread::msleep(500);
        }
    }

//    path.clear();
}

void MoveInPathAction::set_current(Tile* current)
{
    this->current = current;
}

void MoveInPathAction::set_path(vector<Tile*> path)
{
    this->path = path;
}
