#include "Engine.h"

#include "GfxMgr.h"
#include "InputMgr.h"
#include "SoundMgr.h"
#include "GameMgr.h"
#include "UIMgr.h"

UIMgr::UIMgr(Engine* engine) :
	Mgr(engine), m_tray_mgr(nullptr), m_overlay_system(nullptr), m_next_button(nullptr), m_enemy_counter(nullptr), m_levels(nullptr), m_health_bar(nullptr) {
}

UIMgr::~UIMgr(void) {
	Stop();
}

void UIMgr::Init(void) {
	m_overlay_system = new Ogre::OverlaySystem();
	m_engine->GetGfxMgr()->GetOgreSceneManager()->addRenderQueueListener(m_overlay_system);

	m_flight_sound = "assets/sounds/inflight.ogg";
	m_intro_music = "assets/sounds/intro1.ogg";
}

void UIMgr::LoadLevel(void) {
	m_input_context.mKeyboard = m_engine->GetInputMgr()->GetOISKeyboard();
	m_input_context.mMouse = m_engine->GetInputMgr()->GetOISMouse();
	m_tray_mgr = new OgreBites::SdkTrayManager("InterfaceName", m_engine->GetGfxMgr()->GetOgreRenderWindow(), m_input_context, this);

	m_tray_mgr->showBackdrop("Intro/UI");

	m_next_button = m_tray_mgr->createButton(OgreBites::TL_BOTTOMRIGHT, "Next_Button", "next");

	m_tray_mgr->createButton(OgreBites::TL_BOTTOMLEFT, "Credits", "View Credits");
	m_tray_mgr->createButton(OgreBites::TL_BOTTOMLEFT, "Exit", "Exit Game!");
	m_enemy_counter = m_tray_mgr->createLabel(OgreBites::TL_BOTTOMLEFT, "EnemyCounter", "Enemy Counter: ");
	m_levels = m_tray_mgr->createLabel(OgreBites::TL_BOTTOMLEFT, "LevelsWonCounter", "Levels won: ");

	m_engine->GetSoundMgr()->LoadAudio(m_intro_music, m_intro_music);
	m_engine->GetSoundMgr()->LoadAudio(m_engine->GetUIMgr()->m_flight_sound, m_engine->GetUIMgr()->m_flight_sound);

	m_engine->GetSoundMgr()->PlayAudio(m_intro_music);
	m_tray_mgr->getCursorImage()->hide();
}

void UIMgr::Tick(float dt) {
#if 0
	static int x = 0;
	if(++x == 1)
	buttonHit(m_next_button);
#endif

	m_tray_mgr->refreshCursor();

	if (m_health_bar && GetHealthPercentage() <= 0.1)
		m_engine->GetGameMgr()->CrashPlane();

	m_enemy_counter->setCaption("Enemy Counter: " + std::to_string(m_engine->GetGameMgr()->GetNumEnemies()));
	m_levels->setCaption("Levels Won: " + std::to_string(m_engine->GetGameMgr()->GetLevelsWon()));
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
	if (!m_health_bar)
		return -1;
	return 100 * m_health_bar->getProgress();
}

void UIMgr::SetHealthBarPercentage(float percentage, bool accumulate) {
	if (!m_health_bar)
		return;
	if (accumulate)
		m_health_bar->setProgress(m_health_bar->getProgress() + percentage / 100);
	else
		m_health_bar->setProgress(percentage / 100);
}

void UIMgr::PlayFlightSound(void) {
	m_engine->GetSoundMgr()->StopAllAudio();
	m_engine->GetSoundMgr()->PlayAudio(m_engine->GetUIMgr()->m_flight_sound);
}

void UIMgr::ShowBackdrop(void) {
	m_tray_mgr->showBackdrop("Credits/UI");
}

void UIMgr::buttonHit(OgreBites::Button *b) {
	if (b->getName() == "Credits") {
		std::cout << "Showing Credits" << std::endl;
		m_tray_mgr->showOkDialog("Credits", "This game was created by Deev Patel (Physics, AI and GfxMgr) and Cody Ryan (Sound, UI, GameMgr).");
	}

	else if (b->getName() == "Exit") {
		std::cout << "Exiting Game!" << std::endl;
		m_tray_mgr->showBackdrop("Credits/UI");
		m_engine->StopRunning();
	}

	else if (b->getName() == "Next_Button") {
		m_engine->GetGameMgr()->WinLevel();

		m_engine->GetSoundMgr()->StopAllAudio();

		m_flight_sound = "assets/sounds/inflight.ogg";
		m_engine->GetSoundMgr()->LoadAudio(m_flight_sound, m_flight_sound);
		m_engine->GetSoundMgr()->PlayAudio(m_flight_sound);

		m_health_bar = m_tray_mgr->createProgressBar(OgreBites::TL_TOP, "HealthBar", "Health", 500, 400);
		SetHealthBarPercentage(100);

		m_engine->GetGameMgr()->SetLevelsWon(0);

		//Need to figure out a way to delete the next button completely. Right now it appears in the upper left part of the screen and causes an error when pressed
		b->hide();
		m_tray_mgr->clearTray(OgreBites::TL_BOTTOMRIGHT);
		m_tray_mgr->hideBackdrop();
	}
}
