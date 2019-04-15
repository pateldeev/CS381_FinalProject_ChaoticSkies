#ifndef GAMEMGR_H_
#define GAMEMGR_H_

#include <OGRE/OgreSceneNode.h>

#include "Mgr.h"

class Entity381;

class GameMgr: public Mgr {
public:
	GameMgr(Engine *engine);
	virtual ~GameMgr(void);

public:
	virtual void Init(void) override;
	virtual void LoadLevel(void) override;
	virtual void Tick(float dt) override;
	virtual void Stop(void) override;

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

	void SetCameraStateToDefault(void);

private:
	Ogre::SceneNode *m_camera_node;
	Entity381 *m_camera_following;

	//used to keep track of keys pressed for manual camera movement and changing desired
	OIS::KeyCode m_camera_manual_control;
	OIS::KeyCode m_desired_control;
};

#endif
