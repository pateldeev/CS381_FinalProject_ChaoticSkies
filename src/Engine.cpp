#include "Engine.h"

#include "GfxMgr.h"
#include "InputMgr.h"
#include "SoundMgr.h"
#include "GameMgr.h"
#include "EntityMgr.h"

Engine::Engine(void) :
	m_gfx_mgr(nullptr), m_input_mgr(nullptr), m_sound_mgr(nullptr), m_game_mgr(nullptr), m_entity_mgr(nullptr), m_running(false) {
}

Engine::~Engine(void) {
	Cleanup();

	delete m_entity_mgr;
	delete m_game_mgr;
	delete m_sound_mgr;
	delete m_input_mgr;
	delete m_gfx_mgr;
}

void Engine::Init(void) {
	m_gfx_mgr = new GfxMgr(this);
	m_input_mgr = new InputMgr(this);
	m_sound_mgr = new SoundMgr(this);
	m_game_mgr = new GameMgr(this);
	m_entity_mgr = new EntityMgr(this);

	m_gfx_mgr->Init();
	m_input_mgr->Init();
	m_sound_mgr->Init();
	m_game_mgr->Init();
	m_entity_mgr->Init();

	m_gfx_mgr->LoadLevel();
	m_input_mgr->LoadLevel();
	m_sound_mgr->LoadLevel();
	m_game_mgr->LoadLevel();
	m_entity_mgr->LoadLevel();

	m_running = true;
}

void Engine::Run(void) {
	const static float MICROSECONDS_PER_SECOND = 1000000.0f;
	Ogre::Timer* timer = new Ogre::Timer();

	float new_time, old_time = timer->getMicroseconds() / MICROSECONDS_PER_SECOND;
	float dt = 0;

	while (m_running) {
		TickAll(dt);

		new_time = timer->getMicroseconds() / MICROSECONDS_PER_SECOND;
		dt = new_time - old_time;
		old_time = new_time;
	}
}

void Engine::Cleanup(void) {
	m_entity_mgr->Stop();
	m_game_mgr->Stop();
	m_sound_mgr->Stop();
	m_input_mgr->Stop();
	m_gfx_mgr->Stop();
}

GfxMgr* Engine::GetGfxMgr(void) {
	return m_gfx_mgr;
}

const GfxMgr* Engine::GetGfxMgr(void) const {
	return m_gfx_mgr;
}

InputMgr* Engine::GetIngputMgr(void) {
	return m_input_mgr;
}

const InputMgr* Engine::GetIngputMgr(void) const {
	return m_input_mgr;
}

SoundMgr* Engine::GetSoundMgr(void) {
	return m_sound_mgr;
}

const SoundMgr* Engine::GetSoundMgr(void) const {
	return m_sound_mgr;
}

GameMgr* Engine::GetGameMgr(void) {
	return m_game_mgr;
}

const GameMgr* Engine::GetGameMgr(void) const {
	return m_game_mgr;
}

EntityMgr* Engine::GetEntityMgr(void) {
	return m_entity_mgr;
}

const EntityMgr* Engine::GetEntityMgr(void) const {
	return m_entity_mgr;
}

bool Engine::IsRunning(void) const {
	return m_running;
}

void Engine::StopRunning(void) {
	m_running = false;
}

void Engine::TickAll(float dt) {
	m_gfx_mgr->Tick(dt);
	m_input_mgr->Tick(dt);
	m_sound_mgr->Tick(dt);
	m_game_mgr->Tick(dt);
	m_entity_mgr->Tick(dt);
}

