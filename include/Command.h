#ifndef __COMMAND_H_
#define __COMMAND_H_

class Entity381;

class Command {
public:
	virtual ~Command(void);

public:
	virtual void Tick(float dt) = 0;

	virtual void Init(void);
	virtual void Stop(void);

	virtual void DrawBoundingBox(bool draw_all = false) const;

public:
	bool Done(void) const;

protected:
	Command(Entity381 *parent); //only meant to be created by sub classes

protected:
	Entity381 *m_parent;
	bool m_running;
};

#include "CommandSubClasses/CommandMoveTo.h"
#include "CommandSubClasses/CommandRoll.h"
#include "CommandSubClasses/CommandDie.h"

#endif
