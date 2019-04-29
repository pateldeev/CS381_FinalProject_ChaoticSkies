#ifndef GAMEMGR_H_
#define GAMEMGR_H_

#include <OGRE/OgreSceneNode.h>

#include "Mgr.h"

#include "Bullet.h"

class Entity381;

class GameMgr: public Mgr {
public:
	GameMgr(Engine *engine);
	virtual ~GameMgr(void);

public:
	virtual void Init(void) override final;
	virtual void LoadLevel(void) override final;
	virtual void Tick(float dt) override final;
	virtual void Stop(void) override final;

public:
	void InjectKeyPress(const OIS::KeyCode& key);
	void InjectKeyRelease(const OIS::KeyCode &key);

private:
	void MakeCamera(void);
	void MakeGround(void);
	void MakeSky(void);
	void MakeLighting(void);
	void MakeEntities(void);

	void UpdateSelectedDesiredAtributes(float dt);

	void UpdateCamera(float dt);
	void UpdateCameraToFollowEntity(void);

	void HandleBulletsAndFiring(float dt);

	void SetCameraStateToDefault(void);

	void ResetLevel(void);
private:
	Ogre::SceneNode* m_camera_node;
	Entity381* m_camera_following;

	Entity381* m_plane;
	std::list<Entity381*> m_enemies;

	//used to keep track of keys pressed for manual camera movement and changing desired
	OIS::KeyCode m_camera_manual_control;
	OIS::KeyCode m_desired_control;

	float m_fire_cooldown;
	std::list<Bullet*> m_bullets;

	std::string m_flight_sound;
	std::string m_bullet_sound;
};

#endif
