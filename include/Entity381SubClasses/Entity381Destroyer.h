#ifndef __ENTITY381DESTROYER_H_
#define __ENTITY381DESTROYER_H_

#include "Entity381.h"

class Entity381Destroyer: public Entity381 {
public:
	Entity381Destroyer(Engine *engine, const std::string &mesh, const Ogre::Vector3 &pos = Ogre::Vector3::ZERO);
	virtual ~Entity381Destroyer(void);
};

#endif
