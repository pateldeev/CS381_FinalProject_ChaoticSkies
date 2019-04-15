#ifndef __COMMANDINTERCEPT_H_
#define __COMMANDINTERCEPT_H_

#include "CommandSubClasses/CommandMoveTo.h"

class CommandIntercept: public CommandMoveTo {
public:
	CommandIntercept(Entity381 *parent, const Entity381 *target);
	virtual ~CommandIntercept(void);

public:
	virtual void Tick(float dt) override final;

	virtual void DrawBoundingBox(bool draw_all = false) const override final;

private:
	const Entity381* m_intercept_target;
};

#endif
