#ifndef COPSAIAGENT_H
#define COPSAIAGENT_H

#include "aiagent.h"
#include "astar.h"

class CopsAiAgent : public AiAgent
{
public:
    CopsAiAgent(Game *g) : AiAgent(g, COP), astar(g) {}
    void run();
    Tile* lock_target();
    void capture(AiAgent* agent);

    void set_current(Tile* current);

    Tile* get_current();
    void set_target(Tile* target);
    ~CopsAiAgent();
private:
    Tile* current;
    Tile* target;
    AStar astar;
};

#endif // COPSAIAGENT_H
