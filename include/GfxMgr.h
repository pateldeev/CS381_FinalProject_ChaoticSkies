#ifndef GFXMGR_H_
#define GFXMGR_H_

#include <OGRE/OgreRoot.h>
#include <OGRE/OgreWindowEventUtilities.h>

#include <OGRE/SdkTrays.h>
#include <OGRE/Overlay/OgreOverlaySystem.h>

#include "Mgr.h"

class GfxMgr: public Mgr, public Ogre::WindowEventListener {
public:
	GfxMgr(Engine *engine);
	virtual ~GfxMgr(void);

public:
	virtual void Init(void) override final;
	virtual void LoadLevel(void) override final;
	virtual void Tick(float dt) override final;
	virtual void Stop(void) override final;

	virtual void windowClosed(Ogre::RenderWindow* rw) override final;
	virtual void windowResized(Ogre::RenderWindow* rw) override final;

public:
	Ogre::SceneManager* GetOgreSceneManager(void);
	const Ogre::SceneManager* GetOgreSceneManager(void) const;

	Ogre::RenderWindow* GetOgreRenderWindow(void);
	const Ogre::RenderWindow* GetOgreRenderWindow(void) const;

	Ogre::Camera* GetOgreCamera(void);
	const Ogre::Camera* GetOgreCamera(void) const;

	Ogre::Ray CreateOgreRay(float x, float y) const;

private:
	Ogre::Root* m_root;
	Ogre::String m_resources_file;
	Ogre::String m_plugins_file;
	Ogre::RenderWindow* m_window;
	Ogre::SceneManager* m_scene_mgr;
	Ogre::Camera* m_camera;
};

#endif
