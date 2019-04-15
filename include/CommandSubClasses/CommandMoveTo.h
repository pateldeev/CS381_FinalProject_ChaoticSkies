#ifndef __COMMANDMOVETO_H_
#define __COMMANDMOVETO_H_

#include <OGRE/OgreVector3.h>

#include "Command.h"

class CommandMoveTo: public Command {
public:
	CommandMoveTo(Entity381 *parent, const Ogre::Vector3 &position);
	virtual ~CommandMoveTo(void);

public:
	virtual void Tick(float dt) override;
	virtual void Init(void) override final;

protected:
	Ogre::Vector3 m_target;
	float m_distance_to_target;

private:
	static float acceptable_error;
};

#include "CommandSubClasses/CommandFollow.h"
#include "CommandSubClasses/CommandIntercept.h"

#endif
