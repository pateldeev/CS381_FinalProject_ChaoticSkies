#ifndef __COMMANDPATROL_H_
#define __COMMANDPATROL_H_

#include <OGRE/OgreVector3.h>

#include "Command.h"

class CommandPatrol: public Command {
public:
	CommandPatrol(Entity381* parent, const Ogre::Vector3 &center, const Ogre::Vector3 &extent = Ogre::Vector3(500, 15, 500));
	virtual ~CommandPatrol(void);

public:
	virtual void Tick(float dt) override final;
	virtual void Init(void) override final;

private:
	void EnsureRollAndPitchReasonable(void);

	void UpdateYawDirection(void);
	void UpdatePitchDirection(void);
	void UpdateRollDirection(void);

private:
	Ogre::Vector3 m_center;
	Ogre::Vector3 m_extent;

	int m_yaw_direction;
	int m_pitch_direction;
	int m_roll_direction;
};

#endif
