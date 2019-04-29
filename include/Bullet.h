#ifndef __BULLET_H_
#define __BULLET_H_

#include <OGRE/OgreVector3.h>

class Engine;

class Bullet {
public:
	Bullet(Engine* engine, const Ogre::Vector3 &pos, const Ogre::Vector3 &direction, float speed = 1000, float active_time = 30);
	virtual ~Bullet(void);

public:
	void Tick(float dt);

	void Deactivate(void);

	bool HasCollidedWithInLastTick(const Ogre::Vector3 &pos, float radius = 25) const;

public:
	float GetSpeed(void) const;
	void SetSpeed(float speed);

	unsigned int GetId(void) const;
	std::string GetName(void) const;

	Ogre::Vector3 GetPosition(void) const;

	bool IsStillActive(void) const;

private:
	static unsigned int id_generator;

private:
	Engine* m_engine;

	const unsigned int m_id;
	const std::string m_name;

	float m_active_time_left;

	Ogre::Entity* m_ogre_entity;
	Ogre::SceneNode* m_scene_node;

	Ogre::Vector3 m_direction;
};

#endif
