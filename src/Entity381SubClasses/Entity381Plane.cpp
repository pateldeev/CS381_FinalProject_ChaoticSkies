#include "Entity381SubClasses/Entity381Plane.h"

#include "Utils.h"

Entity381Plane::Entity381Plane(Engine *engine, const std::string &mesh, const Ogre::Vector3 &pos) :
	Entity381(engine, mesh, true, "assets/sounds/Selection_Airplane.ogg", pos, Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_Y)) {

	m_speed_max = 250;
	m_acceleration = 30;
	m_turn_rate = 10;

	m_yaw_rate_max = 25;
	m_pitch_rate_max = 11.5;
	m_roll_rate_max = 50;
}

Entity381Plane::~Entity381Plane(void) {
}
