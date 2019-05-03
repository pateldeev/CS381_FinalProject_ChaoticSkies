#ifndef __COMMANDPITCH_H_
#define __COMMANDPITCH_H_

#include "Command.h"

class CommandPitch: public Command {
public:
	CommandPitch(Entity381* parent, bool pitch_up, float pitch_rate = 200);
	virtual ~CommandPitch(void);

public:
	virtual void Tick(float dt) override final;
	virtual void Init(void) override final;

private:
	float m_pitch_speed;
	float m_pitch_target;
};

#endif
