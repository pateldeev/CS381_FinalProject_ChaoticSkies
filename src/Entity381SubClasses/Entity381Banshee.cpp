#include "Entity381SubClasses/Entity381Banshee.h"

Entity381Banshee::Entity381Banshee(Engine *engine, const std::string &mesh, const Ogre::Vector3 &pos) :
	Entity381(engine, mesh, true, "assets/sounds/Selection_Banshee.ogg", pos) {

	m_speed_max = 250.0f;
	m_acceleration = 30.0f;
	m_turn_rate = 45.0f;

	m_pitch_rate_max = 12.f;
	m_roll_rate_max = 12.f;
}

Entity381Banshee::~Entity381Banshee(void) {
}

void Entity381Banshee::GetMeshOrientationsFixed(float &yaw, float &pitch, float &roll) const {
	float temp = pitch;
	pitch = roll;
	roll = temp;
}
