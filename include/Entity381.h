#ifndef __ENTITY381_H_
#define __ENTITY381_H_

#include <OGRE/OgreEntity.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreVector3.h>

#include "Engine.h"

#include "Aspect.h"

class Entity381 {
public:
	virtual ~Entity381(void);

public:
	void Tick(float dt);

	void LoadAudio(void);
	void MakeSelectionSound(void);

	void RemoveAllCommands(void);
	void AddCommand(Command *c, bool remove_past = false);

	void AddSmoke(void);

	virtual void GetMeshOrientationsFixed(float &yaw, float &pitch, float &roll) const;

	void ShowBoundingBox(void) const;
	void HideBoundingBox(void) const;

public:
	unsigned int GetId(void) const;
	std::string GetName(void) const;

	float GetSpeed(void) const;
	float GetSpeedDesired(void) const;
	void SetSpeedDesired(float speed, bool accumulate = false);

	float GetHeading(void) const;
	float GetHeadingDesired(void) const;
	void SetHeadingDesired(float heading, bool accumulate = false);

	float GetPitch(void) const;
	void PitchUp(void);
	void PitchDown(void);
	void PitchStop(void);

	float GetRoll(void) const;
	void RollLeft(void);
	void RollRight(void);
	void RollStop(void);

	Ogre::SceneNode* GetOgreSceneNode(void);
	const Ogre::SceneNode* GetOgreSceneNode(void) const;

	Ogre::Vector3 GetPosition(void) const;
	Ogre::Vector3 GetVelocity(void) const;
	Ogre::Vector3 GetDirection(void) const; //returns direction of mesh
	Ogre::Quaternion GetRotation(void) const; //returns rotation of mesh

	const AspectUnitAI* GetAIAspect(void) const;

	float GetSpeedMax(void) const;
	float GetSpeedMin(void) const;

protected:
	Entity381(Engine *engine, const std::string &mesh, bool apply_3Dphysics = false, const std::string &selection_sound_file = "",
		const Ogre::Vector3 &pos = Ogre::Vector3::ZERO, const Ogre::Quaternion &rotate = Ogre::Quaternion::IDENTITY);

protected:
	float m_speed_min;
	float m_speed_max;
	float m_acceleration;

	float m_turn_rate;

	float m_pitch_rate_max;
	float m_roll_rate_max;
private:
	Engine *m_engine;

	const unsigned int m_id;
	const std::string m_name;

	const std::string m_mesh_file;

	std::string m_selection_sound;

	float m_speed;
	float m_speed_desired;

	float m_heading;
	float m_heading_desired;

	float m_pitch;
	float m_pitch_rate_current;
	float m_roll;
	float m_roll_rate_current;

	Ogre::Entity* m_ogre_entity;
	Ogre::SceneNode* m_scene_node;
	Ogre::SceneNode* m_smoke_node;

	Ogre::Vector3 m_position;
	Ogre::Vector3 m_velocity;

	AspectUnitAI *m_AI_aspect;
	std::vector<Aspect*> m_aspects;

private:
	friend class AspectPhysics2D;
	friend class AspectPhysics3D;
	friend class CommandRoll;

private:
	static unsigned int id_generator;
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
