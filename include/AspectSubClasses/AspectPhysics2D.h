#ifndef __ASPECTPHYSICS2D_H_
#define __ASPECTPHYSICS2D_H_

#include "Aspect.h"

class AspectPhysics2D: public Aspect {
public:
	AspectPhysics2D(Entity381 *entity);
	~AspectPhysics2D(void);

public:
	virtual void Tick(float dt) override final;
};

#endif
