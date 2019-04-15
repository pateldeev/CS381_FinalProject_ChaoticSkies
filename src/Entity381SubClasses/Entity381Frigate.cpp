#include "Entity381SubClasses/Entity381Frigate.h"

Entity381Frigate::Entity381Frigate(Engine *engine, const std::string &meshfilename, int identity, const Ogre::Vector3 &pos) :
	Entity381(engine, meshfilename, identity, pos) {
	this->m_speed_min = 0;
	this->m_speed_max = 30.0f; //meters per second...
	this->m_acceleration = 10.0f; // slow
	this->m_turn_rate = 20.0f; //2 degrees per second
	m_selection_sound = "assets/sounds/Selection_Frigate.ogg";

	m_aspects.push_back(new AspectPhysics2D(this));
}

Entity381Frigate::~Entity381Frigate(void) {
}
