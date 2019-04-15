#include "Entity381SubClasses/Entity381Carrier.h"

Entity381Carrier::Entity381Carrier(Engine *engine, const std::string &meshfilename, int identity, const Ogre::Vector3 &pos) :
	Entity381(engine, meshfilename, identity, pos) {
	this->m_speed_min = 0;
	this->m_speed_max = 40.0f; //meters per second...
	this->m_acceleration = 2.0f; // slow
	this->m_turn_rate = 10.0f; //2 degrees per second
	m_selection_sound = "assets/sounds/Selection_Carrier.ogg";

	m_aspects.push_back(new AspectPhysics2D(this));
}

Entity381Carrier::~Entity381Carrier(void) {
}
