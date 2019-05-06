#ifndef __COMMANDROLL_H_
#define __COMMANDROLL_H_

#include "Command.h"

class CommandRoll: public Command {
public:
	CommandRoll(Entity381* parent, bool roll_right, const Ogre::Degree &roll_rate = Ogre::Degree(400));
	virtual ~CommandRoll(void);

public:
	virtual void Tick(float dt) override final;
	virtual void Init(void) override final;

private:
	Ogre::Degree m_roll_rate;
	Ogre::Degree m_roll_target;
};

#endif
