#ifndef __ASPECTPHYSICS3D_H_
#define __ASPECTPHYSICS3D_H_

#include "Aspect.h"

class AspectPhysics3D: public Aspect {
public:
	AspectPhysics3D(Entity381 *entity);
	~AspectPhysics3D(void);

public:
	virtual void Tick(float dt) override final;
};
#endif
