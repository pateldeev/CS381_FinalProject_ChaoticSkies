#include "Entity381SubClasses/Entity381Plane.h"

#include "Utils.h"

Entity381Plane::Entity381Plane(Engine *engine, const std::string &mesh, const Ogre::Vector3 &pos) :
	Entity381(engine, mesh, true, "assets/sounds/Selection_Airplane.ogg", pos, Ogre::Quaternion(-0.7071068, 0.f, 0.7071068, 0.f)) {

	m_speed_max = 250.0f;
	m_acceleration = 30.0f;
	m_turn_rate = 10.f;

	m_pitch_rate_max = 11.5;
	m_roll_rate_max = 50;
}

Entity381Plane::~Entity381Plane(void) {
}

void Entity381Plane::GetMeshOrientationsFixed(float &yaw, float &pitch, float &roll) const {
	yaw += 90;
	yaw = FixAngle(yaw);
	pitch = -pitch;
	roll = -roll;
}

