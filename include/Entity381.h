#ifndef __ENTITY381_H_
#define __ENTITY381_H_

#include <OGRE/OgreEntity.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreVector3.h>

#include "Engine.h"

#include "Aspect.h"

class Entity381 {
public:
	Entity381(Engine *engine, const std::string &meshfilename, int identity, const Ogre::Vector3 &pos = Ogre::Vector3::ZERO,
		const Ogre::Quaternion &rotate = Ogre::Quaternion::IDENTITY);
	virtual ~Entity381(void);

public:
	void Tick(float dt);

	void MakeSelectionSound(void);

	std::string GetName(void) const;

	void RemoveAllCommands(void);
	void AddCommand(Command *c, bool remove_past = false);

	virtual void FixMeshOrientation(float &yaw, float &pitch, float &roll);

public:
	Engine *m_engine;

	int m_id;
	std::string m_name;
	std::string m_mesh_file;

	Ogre::SceneNode* m_scene_node;
	Ogre::Entity* m_ogre_entity;

	float m_acceleration;
	float m_turn_rate;
	float m_speed_min;
	float m_speed_max;

	std::string m_selection_sound;
	unsigned int m_audio_id;

	Ogre::Vector3 m_position;
	Ogre::Vector3 m_velocity;

	float m_desired_heading;
	float m_desired_speed;
	float m_desired_pitch;
	float m_desired_roll;

	float m_heading;
	float m_speed;
	float m_pitch;
	float m_roll;

	AspectUnitAI *m_aspect_ai;

protected:
	std::vector<Aspect*> m_aspects;
};

enum Entity381Types {
	CarrierType, DestroyerType, SpeedBoatType, FrigateType, AlienType, BansheeType, PlaneType
};
#include "Entity381SubClasses/Entity381Carrier.h"
#include "Entity381SubClasses/Entity381Destroyer.h"
#include "Entity381SubClasses/Entity381SpeedBoat.h"
#include "Entity381SubClasses/Entity381Frigate.h"
#include "Entity381SubClasses/Entity381Alien.h"
#include "Entity381SubClasses/Entity381Banshee.h"
#include "Entity381SubClasses/Entity381Plane.h"

#endif
