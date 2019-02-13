#ifndef ACTION_H
#define ACTION_H

#include <game.h>
#include <QThread>

class Action
{
public:
    Action(Game* g) : game(g) {}
    virtual void execute() = 0;
    Game* game;
private:
};

#endif // ACTION_H
