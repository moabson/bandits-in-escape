#ifndef BANDITAIAGENT_H
#define BANDITAIAGENT_H

#include "aiagent.h"
#include "astar.h"
#include <vector>

using namespace std;

class BanditAiAgent : public AiAgent
{
public:
    BanditAiAgent(Game *g) : AiAgent(g, BANDIT), astar(g) {}
    void run();
    vector<Tile*>& lock_target();
    void set_current(Tile* current);
    Tile* get_current();
    void set_target(Tile* target);
    void set_path(vector<Tile*> path);
    ~BanditAiAgent();
private:
    vector<Tile*> path;
    Tile* current;
    Tile* target;
    AStar astar;
};

#endif // BANDITAIAGENT_H
