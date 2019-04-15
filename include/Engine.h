#ifndef ENGINE_H_
#define ENGINE_H_

class GfxMgr;
class InputMgr;
class SoundMgr;
class GameMgr;
class EntityMgr;

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

	InputMgr* GetIngputMgr(void);
	const InputMgr* GetIngputMgr(void) const;

	SoundMgr* GetSoundMgr(void);
	const SoundMgr* GetSoundMgr(void) const;

	GameMgr* GetGameMgr(void);
	const GameMgr* GetGameMgr(void) const;

	EntityMgr* GetEntityMgr(void);
	const EntityMgr* GetEntityMgr(void) const;

	bool IsRunning(void) const;
	void StopRunning(void);

private:
	void TickAll(float dt);

private:
	GfxMgr* m_gfx_mgr;
	InputMgr* m_input_mgr;
	SoundMgr* m_sound_mgr;
	GameMgr* m_game_mgr;
	EntityMgr* m_entity_mgr;

	bool m_running;
};

#endif
