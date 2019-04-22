#ifndef __ENTITY381CARRIER_H_
#define __ENTITY381CARRIER_H_

#include "Entity381.h"

class Entity381Carrier: public Entity381 {
public:
	Entity381Carrier(Engine *engine, const std::string &mesh, int id, const Ogre::Vector3 &pos = Ogre::Vector3::ZERO);
	virtual ~Entity381Carrier(void);
};

#endif
