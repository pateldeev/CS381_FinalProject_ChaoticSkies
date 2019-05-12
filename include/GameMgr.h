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

public:
	void CrashPlane(void);
	void WinLevel(void);
	void LoseLevel(void);
	void ResetLevel(void);

public:
	int GetLevelsWon(void) const;
	void SetLevelsWon(int levels);

	unsigned int GetNumEnemies(void) const;
	unsigned int GetNumEnemiesFollowing(void) const;

	//only should be called by command patrol
	void AddEnemyFollowing(void);
	void RemoveEnemyFollowing(void);

private:
	void MakeCamera(void);
	void MakeGround(void);
	void MakeSky(void);
	void MakeLighting(void);
	void MakeEntities(void);
	void MakeBoats(void);
	void MakePlaneMain(int speed);
	void MakeEnemies(int level);

	void UpdateSelectedDesiredAtributes(float dt);

	void UpdateCamera(float dt);
	void UpdateCameraToFollowEntity(void);

	void HandleBulletsAndFiring(float dt);
	void RemoveDeadEntities(void);

	void SetCameraStateToDefault(void);

private:
	Ogre::SceneNode* m_camera_node;
	Entity381* m_camera_following;

	Entity381* m_plane;
	std::list<Entity381*> m_enemies;
	unsigned int m_enemies_following;

	//used to keep track of keys pressed for manual camera movement and changing desired
	OIS::KeyCode m_camera_manual_control;
	OIS::KeyCode m_desired_control;

	float m_fire_cooldown;
	std::list<Bullet*> m_bullets;

	std::string m_explosion_sound;
	std::string m_bullet_sound;

	int m_levels_won;

private:
	static float fire_cooldown;
};

#endif
