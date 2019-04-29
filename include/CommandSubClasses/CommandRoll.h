#ifndef __COMMANDROLL_H_
#define __COMMANDROLL_H_

#include <OGRE/OgreVector3.h>

#include "Command.h"

class CommandRoll: public Command {
public:
	CommandRoll(Entity381 *parent, bool roll_right);
	virtual ~CommandRoll(void);

public:
	virtual void Tick(float dt) override;
	virtual void Init(void) override final;

private:
	float m_roll_rate;
	float m_roll_target;
};

#endif
