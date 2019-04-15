#ifndef __ASPECT_H_
#define __ASPECT_H_

#include <OGRE/OgreVector3.h>

#include "Command.h"

class Entity381;

class Aspect {
public:
	Aspect(Entity381* entity);
	virtual ~Aspect(void);

public:
	virtual void Tick(float dt) = 0;

protected:
	Entity381* m_entity;
};

#include "AspectSubClasses/AspectRenderable.h"
#include "AspectSubClasses/AspectPhysics2D.h"
#include "AspectSubClasses/AspectPhysics3D.h"
#include "AspectSubClasses/AspectUnitAI.h"

#endif
