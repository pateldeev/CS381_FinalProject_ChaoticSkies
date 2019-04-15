#include "Entity381SubClasses/Entity381Banshee.h"

Entity381Banshee::Entity381Banshee(Engine *engine, const std::string &meshfilename, int identity, const Ogre::Vector3 &pos) :
	Entity381(engine, meshfilename, identity, pos) {
	this->m_speed_min = 0;
	this->m_speed_max = 250.0f; //meters per second...
	this->m_acceleration = 30.0f; // slow
	this->m_turn_rate = 45.0f; //2 degrees per second

	m_selection_sound = "assets/sounds/Selection_Banshee.ogg";

	m_aspects.push_back(new AspectPhysics2D(this));
}

Entity381Banshee::~Entity381Banshee(void) {
}

void Entity381Banshee::FixMeshOrientation(float &yaw, float &pitch, float &roll) {
	float temp = pitch;
	pitch = roll;
	roll = temp;
}
