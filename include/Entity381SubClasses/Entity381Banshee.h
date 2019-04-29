#ifndef __ENTITY381BANSHEE_H_
#define __ENTITY381BANSHEE_H_

#include "Entity381.h"

class Entity381Banshee: public Entity381 {
public:
	Entity381Banshee(Engine *engine, const std::string &mesh, const Ogre::Vector3 &pos = Ogre::Vector3::ZERO);
	virtual ~Entity381Banshee(void);

public:
	virtual void GetMeshOrientationsFixed(float &yaw, float &pitch, float &roll) const override final;
};

#endif
