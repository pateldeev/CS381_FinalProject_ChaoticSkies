#ifndef __COMMANDPATROL_H_
#define __COMMANDPATROL_H_

#include <OGRE/OgreVector3.h>

#include "Command.h"

#define SHOW_TARGET 0

#include "Bullet.h"

class CommandPatrol: public Command {
public:
	CommandPatrol(Entity381* parent, Entity381* main_plane, const Ogre::Vector3 &center, const Ogre::Vector3 &extent = Ogre::Vector3(500, 15, 500));
	virtual ~CommandPatrol(void);

public:
	virtual void Tick(float dt) override final;
	virtual void Init(void) override final;

private:
	void EnsureRollAndPitchReasonable(void);

	void UpdateTargetIfClose(void);
	void UpdateTarget(const Ogre::Vector3 &target);

	void UpdateBulletsAndFiring(float dt);

private:
	Ogre::Vector3 m_center;
	Ogre::Vector3 m_extent;

	Ogre::Vector3 m_target;

	Entity381* m_plane;

	std::list<Bullet*> m_bullets;
	float m_firing_cooldown;

#if SHOW_TARGET
	Bullet* m_b = nullptr;
#endif
};

#endif
