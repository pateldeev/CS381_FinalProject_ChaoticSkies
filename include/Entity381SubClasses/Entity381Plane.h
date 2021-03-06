#ifndef __ENTITY381PLANE_H_
#define __ENTITY381PLANE_H_

#include "Entity381.h"

class Entity381Plane: public Entity381 {
public:
	Entity381Plane(Engine *engine, const std::string &mesh, const Ogre::Vector3 &pos = Ogre::Vector3::ZERO);
	virtual ~Entity381Plane(void);
};

#endif
