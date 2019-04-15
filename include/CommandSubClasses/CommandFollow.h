#ifndef __COMMANDFOLLOW_H_
#define __COMMANDFOLLOW_H_

#include "CommandSubClasses/CommandMoveTo.h"

class CommandFollow: public CommandMoveTo {
public:
	CommandFollow(Entity381 *parent, const Entity381 *target);
	virtual ~CommandFollow(void);

public:
	virtual void Tick(float dt) override final;

	virtual void DrawBoundingBox(bool draw_all = false) const override final;

private:
	const Entity381* m_following;

private:
	static float max_follow_distance;
};

#endif
