#ifndef __ENTITY381FRIGATE_H_
#define __ENTITY381FRIGATE_H_

#include "Entity381.h"

class Entity381Frigate: public Entity381 {
public:
	Entity381Frigate(Engine *engine, const std::string &meshfilename, int identity, const Ogre::Vector3 &pos = Ogre::Vector3::ZERO);
	virtual ~Entity381Frigate(void);

};

#endif
