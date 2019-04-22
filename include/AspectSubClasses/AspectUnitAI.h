#ifndef __ASPECTUNITAI_H_
#define __ASPECTUNITAI_H_

#include "Aspect.h"

class AspectUnitAI: public Aspect {
public:
	AspectUnitAI(Entity381 *entity);
	~AspectUnitAI(void);

public:
	virtual void Tick(float dt) override final;

public:
	void SetCommand(Command *c);
	void AddCommand(Command *c);

	void RemoveAllCommands(void);

	void DrawBoundingBox(bool draw_all = false) const;

private:
	std::queue<Command*> m_commands;
};

#endif
