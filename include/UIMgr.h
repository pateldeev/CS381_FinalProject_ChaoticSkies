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
	void SetHealthBarPercentage(float percentage, bool accumulate = false);

	void PlayFlightSound(void);

	void ShowBackdrop(void);

private:
	virtual void buttonHit(OgreBites::Button *b) override final;

private:
	OgreBites::InputContext m_input_context;
	OgreBites::SdkTrayManager* m_tray_mgr;
	Ogre::OverlaySystem* m_overlay_system;

	OgreBites::Button* m_next_button;

	OgreBites::Label* m_enemy_counter;
	OgreBites::Label* m_levels;

	OgreBites::ProgressBar* m_health_bar;

	std::string m_flight_sound;
	std::string m_intro_music;
};

#endif
