#include "Entity381SubClasses/Entity381Banshee.h"

Entity381Banshee::Entity381Banshee(Engine *engine, const std::string &mesh, const Ogre::Vector3 &pos) :
	Entity381(engine, mesh, true, "assets/sounds/Selection_Banshee.ogg", pos) {

	m_speed_max = 250;
	m_acceleration = 30;

	m_yaw_rate_max = 50;
	m_pitch_rate_max = 12;
	m_roll_rate_max = 12;
}

Entity381Banshee::~Entity381Banshee(void) {
}

Ogre::Vector3 Entity381Banshee::GetDirection(void) const {
	return (GetOgreSceneNode()->_getDerivedOrientation() * Ogre::Vector3::UNIT_X).normalisedCopy();
}

Ogre::Quaternion Entity381Banshee::GetRotationLocal(void) const {
	Ogre::Quaternion q = Entity381::GetRotationLocal();
	Ogre::Matrix3 m;
	q.ToRotationMatrix(m);
	Ogre::Radian y, p, r;
	m.ToEulerAnglesYXZ(y, p, r);
	m.FromEulerAnglesYXZ(y, -r, p);
	q.FromRotationMatrix(m);
	return q;
}

void Entity381Banshee::UpdateRotation(float dt) {
	m_rotation = Ogre::Quaternion(GetYawRate() * dt, Ogre::Vector3::UNIT_Y) * m_rotation * Ogre::Quaternion(-GetPitchRate() * dt, Ogre::Vector3::UNIT_Z) * Ogre::Quaternion(GetRollRate() * dt, Ogre::Vector3::UNIT_X);
}
