#ifndef ENGINE_H_
#define ENGINE_H_

class GfxMgr;
class InputMgr;
class SoundMgr;
class UIMgr;
class EntityMgr;
class GameMgr;

class Engine {
public:
	Engine(void);
	virtual ~Engine(void);

public:
	void Init(void);
	void Run(void);
	void Cleanup(void);

public:
	GfxMgr* GetGfxMgr(void);
	const GfxMgr* GetGfxMgr(void) const;

	InputMgr* GetInputMgr(void);
	const InputMgr* GetInputMgr(void) const;

	SoundMgr* GetSoundMgr(void);
	const SoundMgr* GetSoundMgr(void) const;

	UIMgr* GetUIMgr(void);
	const UIMgr* GetUIMgr(void) const;

	EntityMgr* GetEntityMgr(void);
	const EntityMgr* GetEntityMgr(void) const;

	GameMgr* GetGameMgr(void);
	const GameMgr* GetGameMgr(void) const;

public:
	bool IsRunning(void) const;
	void StopRunning(void);

private:
	void TickAll(float dt);

private:
	GfxMgr* m_gfx_mgr;
	InputMgr* m_input_mgr;
	SoundMgr* m_sound_mgr;
	UIMgr* m_UI_mgr;
	EntityMgr* m_entity_mgr;
	GameMgr* m_game_mgr;

	bool m_running;
};

#endif
