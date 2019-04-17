#include "UIMgr.h"

UIMgr::UIMgr(Engine* engine) :
	Mgr(engine), m_tray_mgr(nullptr), m_overlay_system(nullptr), m_label(nullptr), m_info_label(nullptr), m_info_label2(nullptr),
	m_info_label3(nullptr) {

	m_overlay_system = new Ogre::OverlaySystem();
	//engine->gfxMgr->mSceneMgr->addRenderQueueListener(mOverlaySystem);

	//Ogre::WindowEventUtilities::addWindowEventListener(engine->gfxMgr->ogreRenderWindow, this);
}

UIMgr::~UIMgr(void) {
	Stop();
}

void UIMgr::Init(void) {
}

void UIMgr::LoadLevel(void) {
}

void UIMgr::Tick(float dt) {
}

void UIMgr::Stop(void) {
}

void UIMgr::windowResized(Ogre::RenderWindow* rw) {
}

void UIMgr::windowClosed(Ogre::RenderWindow* rw) {
}

bool UIMgr::keyPressed(const OIS::KeyEvent &arg) {
	return true;
}
bool UIMgr::keyReleased(const OIS::KeyEvent &arg) {
	return true;
}
bool UIMgr::mouseMoved(const OIS::MouseEvent &arg) {
	return true;
}
bool UIMgr::mousePressed(const OIS::MouseEvent &me, OIS::MouseButtonID mid) {
	return true;
}
bool UIMgr::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
	return true;
}

void UIMgr::buttonHit(OgreBites::Button *b) {
}

void UIMgr::itemSelected(OgreBites::SelectMenu *m) {
}
