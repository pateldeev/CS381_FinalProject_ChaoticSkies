#ifndef __ENTITY381SPEEDBOAT_H_
#define __ENTITY381SPEEDBOAT_H_

#include "Entity381.h"

class Entity381SpeedBoat: public Entity381 {
public:
	Entity381SpeedBoat(Engine *engine, const std::string &mesh, int id, const Ogre::Vector3 &pos = Ogre::Vector3::ZERO);
	virtual ~Entity381SpeedBoat(void);
};

#endif
