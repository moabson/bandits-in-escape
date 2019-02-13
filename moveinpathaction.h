#ifndef MOVEINPATHACTION_H
#define MOVEINPATHACTION_H

#include "game.h"
#include "action.h"
#include <vector>

using namespace std;

class MoveInPathAction : public Action
{
public:
    MoveInPathAction(Game* g) : Action(g) {}
    void execute();
    void set_current(Tile* current);
    void set_path(vector<Tile*> path);
private:
    Tile* current;
    vector<Tile*> path;
};

#endif // MOVEINPATHACTION_H
