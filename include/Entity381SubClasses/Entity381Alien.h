#ifndef __ENTITY381ALIEN_H_
#define __ENTITY381ALIEN_H_

#include "Entity381.h"

class Entity381Alien: public Entity381 {
public:
	Entity381Alien(Engine *engine, const std::string &meshfilename, int identity, const Ogre::Vector3 &pos = Ogre::Vector3::ZERO);
	virtual ~Entity381Alien(void);

};

#endif
