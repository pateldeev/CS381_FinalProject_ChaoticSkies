#include "Engine.h"

#include "GfxMgr.h"
#include "InputMgr.h"
#include "SoundMgr.h"
#include "GameMgr.h"
#include "EntityMgr.h"
#include "UIMgr.h"

UIMgr::UIMgr(Engine* engine) :
	Mgr(engine), m_tray_mgr(nullptr), m_overlay_system(nullptr), m_health_bar(nullptr), m_label(nullptr), m_info_label(nullptr),
	m_info_label2(nullptr), m_info_label3(nullptr) {
}

UIMgr::~UIMgr(void) {
	Stop();
}

void UIMgr::Init(void) {
	m_overlay_system = new Ogre::OverlaySystem();
	m_engine->GetGfxMgr()->GetOgreSceneManager()->addRenderQueueListener(m_overlay_system);
}

void UIMgr::LoadLevel(void) {
	m_input_context.mKeyboard = m_engine->GetIngputMgr()->GetOISKeyboard();
	m_input_context.mMouse = m_engine->GetIngputMgr()->GetOISMouse();
	m_tray_mgr = new OgreBites::SdkTrayManager("InterfaceName", m_engine->GetGfxMgr()->GetOgreRenderWindow(), m_input_context, this);

	m_tray_mgr->createButton(OgreBites::TL_TOPLEFT, "Credits", "View Credits");
	m_tray_mgr->createButton(OgreBites::TL_TOPLEFT, "Exit", "Exit Game!");

	m_health_bar = m_tray_mgr->createProgressBar(OgreBites::TL_TOP, "HealthBar", "Health", 500, 400);
	SetHealthBarPercentage(100);

	m_tray_mgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
	m_tray_mgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
	m_tray_mgr->getCursorImage()->hide();
}

void UIMgr::Tick(float dt) {
	m_tray_mgr->refreshCursor();

	float percentage = GetHealthPercentage();
	if (percentage <= 0)
		SetHealthBarPercentage(100);
	else
		SetHealthBarPercentage(percentage - 0.1 * dt);

	//m_info_label->setCaption("Type: Destroyer");
	//m_info_label2->setCaption("Heading: ");
	//m_info_label3->setCaption("Speed: ");
}

void UIMgr::Stop(void) {
}

bool UIMgr::InjectMouseMovement(const OIS::MouseEvent &me) {
	return m_tray_mgr->injectMouseMove(me);
}

bool UIMgr::InjectMousePress(const OIS::MouseEvent &me, OIS::MouseButtonID id) {
	return m_tray_mgr->injectMouseDown(me, id);
}

bool UIMgr::InjectMouseRelease(const OIS::MouseEvent &me, OIS::MouseButtonID id) {
	return m_tray_mgr->injectMouseUp(me, id);
}

float UIMgr::GetHealthPercentage(void) const {
	return m_health_bar->getProgress();
}

void UIMgr::SetHealthBarPercentage(float percentage) {
	m_health_bar->setProgress(percentage);
}

void UIMgr::buttonHit(OgreBites::Button *b) {
	if (b->getName() == "Credits") {
		std::cout << "Showing Credits" << std::endl;
	} else if (b->getName() == "Exit") {
		std::cout << "Exiting Game!" << std::endl;
		m_engine->StopRunning();
	}
}

void UIMgr::itemSelected(OgreBites::SelectMenu *m) {
}
