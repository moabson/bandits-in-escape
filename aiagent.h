#ifndef AIAGENT_H
#define AIAGENT_H

#include <QThread>
#include <QDebug>

#include <game.h>

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
    AiAgentType get_type();

protected:
    Game* game;
    AiAgentType type;

};

#endif // AIAGENT_H
