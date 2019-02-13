#ifndef BANDITAIAGENT_H
#define BANDITAIAGENT_H

#include "aiagent.h"

class BanditAiAgent : public AiAgent
{
public:
    BanditAiAgent(Game *g) : AiAgent(g, BANDIT) {}
    void run();
    void set_current(Tile* current);
    void set_path(vector<Tile*> path);
    ~BanditAiAgent();
private:
    vector<Tile*> path;
    Tile* current;
};

#endif // BANDITAIAGENT_H
