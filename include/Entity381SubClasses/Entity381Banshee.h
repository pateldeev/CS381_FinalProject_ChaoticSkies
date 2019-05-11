#ifndef __ENTITY381BANSHEE_H_
#define __ENTITY381BANSHEE_H_

#include "Entity381.h"

class Entity381Banshee: public Entity381 {
public:
	Entity381Banshee(Engine *engine, const std::string &mesh, const Ogre::Vector3 &pos = Ogre::Vector3::ZERO);
	virtual ~Entity381Banshee(void);

public:
	virtual Ogre::Vector3 GetDirection(void) const override final;

	virtual Ogre::Quaternion GetRotationLocal(void) const override final;

protected:
	virtual void UpdateRotation(float dt) override final;
};

#endif
