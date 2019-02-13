#ifndef AIAGENT_H
#define AIAGENT_H

#include <QThread>
#include <QDebug>

#include <game.h>
#include "action.h"

enum AiAgentType
{
    BANDIT,
    COP
};

class AiAgent : public QThread
{
public:
    AiAgent(Game* g, AiAgentType t) : game(g), type(t) {}
    virtual ~AiAgent() {}
    vector<Action> get_actions();
    AiAgentType get_type();

protected:
    Game* game;
    AiAgentType type;
    vector<Action> actions;
};

#endif // AIAGENT_H
