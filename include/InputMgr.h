#ifndef INPUTMGR_H_
#define INPUTMGR_H_

#include <OGRE/OgreSceneNode.h>

#include <ois/OISEvents.h>
#include <ois/OISInputManager.h>
#include <ois/OISKeyboard.h>
#include <ois/OISMouse.h>

#include "Mgr.h"

class Entity381;

class InputMgr: public Mgr, public OIS::KeyListener, public OIS::MouseListener {

public:
	InputMgr(Engine *engine);
	virtual ~InputMgr(void);

public:
	virtual void Init(void) override final;
	virtual void LoadLevel(void) override final;
	virtual void Tick(float dt) override final;
	virtual void Stop(void) override final;

	virtual bool keyPressed(const OIS::KeyEvent& key) override final;
	virtual bool keyReleased(const OIS::KeyEvent& key) override final;

	virtual bool mouseMoved(const OIS::MouseEvent& me) override final;
	virtual bool mousePressed(const OIS::MouseEvent& me, OIS::MouseButtonID id) override final;
	virtual bool mouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID id) override final;

public:
	bool IsKeyPressed(const OIS::KeyCode &key) const;

	const OIS::MouseState& GetMouseState(void) const;

private:
	int GetEntityClickedOn(float x, float y) const;

private:
	OIS::InputManager* m_ois_mgr;
	OIS::Keyboard* m_keyboard;
	OIS::Mouse* m_mouse;

	float m_multiselect_start[2];
	float m_multiselect_end[2];

	Ogre::RaySceneQuery* m_ray_query;
};

#endif /* INPUTMGR_H_ */
