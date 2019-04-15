#ifndef __ASPECTRENDERABLE_H_
#define __ASPECTRENDERABLE_H_

#include "Aspect.h"

class AspectRenderable: public Aspect {
public:
	AspectRenderable(Entity381* entity);
	~AspectRenderable(void);

public:
	virtual void Tick(float dt) override final;
};
#endif
