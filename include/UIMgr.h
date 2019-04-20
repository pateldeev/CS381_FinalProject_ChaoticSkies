#ifndef UIMGR_H_
#define UIMGR_H_

#include <OGRE/OgreWindowEventUtilities.h>
#include <OGRE/Overlay/OgreOverlaySystem.h>

#include <OIS/OISEvents.h>
#include <OIS/OISInputManager.h>
#include <OIS/OISKeyboard.h>
#include <OIS/OISMouse.h>

#include <OGRE/SdkTrays.h>

#include <string>

#include "Mgr.h"

class UIMgr: public Mgr, public OgreBites::SdkTrayListener {

public:
	UIMgr(Engine *engine);
	~UIMgr(void);

public:
	virtual void Init(void) override final;
	virtual void LoadLevel(void) override final;
	virtual void Tick(float dt) override final;
	virtual void Stop(void) override final;

public:
	bool InjectMouseMovement(const OIS::MouseEvent &me);
	bool InjectMousePress(const OIS::MouseEvent &me, OIS::MouseButtonID id);
	bool InjectMouseRelease(const OIS::MouseEvent &me, OIS::MouseButtonID id);

public:
	float GetHealthPercentage(void) const;
	void SetHealthBarPercentage(float percentage);

private:
	virtual void buttonHit(OgreBites::Button *b) override final;
	virtual void itemSelected(OgreBites::SelectMenu *m) override final;

private:
	OgreBites::InputContext m_input_context;
	OgreBites::SdkTrayManager* m_tray_mgr;
	Ogre::OverlaySystem* m_overlay_system;

	OgreBites::ProgressBar* m_health_bar;
	OgreBites::Label* m_label;
	OgreBites::Label* m_info_label;
	OgreBites::Label* m_info_label2;
	OgreBites::Label* m_info_label3;
};

#endif
