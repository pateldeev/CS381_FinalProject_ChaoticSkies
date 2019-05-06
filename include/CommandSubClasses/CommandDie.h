#ifndef __COMMANDDIE_H_
#define __COMMANDDIE_H_
#include "Command.h"

class CommandDie: public Command {
public:
	CommandDie(Entity381 *parent);
	virtual ~CommandDie(void);

public:
	virtual void Tick(float dt) override final;
	virtual void Init(void) override final;

private:
	float m_time_left;
};

#endif
