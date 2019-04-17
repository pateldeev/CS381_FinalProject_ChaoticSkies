#ifndef UIMGR_H_
#define UIMGR_H_

#include <OGRE/OgreWindowEventUtilities.h>
#include <OIS/OISEvents.h>
#include <OIS/OISInputManager.h>
#include <OIS/OISKeyboard.h>
#include <OIS/OISMouse.h>

#include <OGRE/SdkTrays.h>

#include <string>

#include "Mgr.h"

class UIMgr: public Mgr,
	public Ogre::FrameListener,
	public OIS::KeyListener,
	public OIS::MouseListener,
	public Ogre::WindowEventListener,
	public OgreBites::SdkTrayListener {

public:
	UIMgr(Engine *engine);
	~UIMgr(void);

public:
	virtual void Init(void) override final;
	virtual void LoadLevel(void) override final;
	virtual void Tick(float dt) override final;
	virtual void Stop(void) override final;

protected:
	virtual void windowResized(Ogre::RenderWindow *rw) override;
	virtual void windowClosed(Ogre::RenderWindow *rw) override;

	virtual bool keyPressed(const OIS::KeyEvent &arg) override;
	virtual bool keyReleased(const OIS::KeyEvent &arg) override;
	virtual bool mouseMoved(const OIS::MouseEvent &arg) override;
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) override;
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) override;

	void buttonHit(OgreBites::Button *b);
	void itemSelected(OgreBites::SelectMenu *m);

private:
	OgreBites::InputContext m_input_context;
	OgreBites::SdkTrayManager* m_tray_mgr;
	Ogre::OverlaySystem* m_overlay_system;
	OgreBites::Label* m_label;
	OgreBites::Label* m_info_label;
	OgreBites::Label* m_info_label2;
	OgreBites::Label* m_info_label3;
};

#endif
