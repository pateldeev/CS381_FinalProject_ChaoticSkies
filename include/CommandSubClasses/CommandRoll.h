#ifndef __COMMANDROLL_H_
#define __COMMANDROLL_H_

#include "Command.h"

class CommandRoll: public Command {
public:
	CommandRoll(Entity381* parent, bool roll_right, float rool_rate = 400);
	virtual ~CommandRoll(void);

public:
	virtual void Tick(float dt) override final;
	virtual void Init(void) override final;

private:
	float m_roll_speed;
	float m_roll_target;
};

#endif
