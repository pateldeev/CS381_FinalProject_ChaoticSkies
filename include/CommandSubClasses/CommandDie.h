#ifndef __COMMANDDIE_H_
#define __COMMANDDIE_H_

#include <OGRE/OgreVector3.h>

#include "Command.h"

class CommandDie: public Command {
public:
	CommandDie(Entity381 *parent);
	virtual ~CommandDie(void);

public:
	virtual void Tick(float dt) override;
	virtual void Init(void) override final;

private:
	float m_pitch_target;
};

#endif