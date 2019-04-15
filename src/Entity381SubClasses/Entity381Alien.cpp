#include "Entity381SubClasses/Entity381Alien.h"

Entity381Alien::Entity381Alien(Engine *engine, const std::string &meshfilename, int identity, const Ogre::Vector3 &pos) :
	Entity381(engine, meshfilename, identity, pos) {
	this->m_speed_min = 0;
	this->m_speed_max = 100.0f; //meters per second...
	this->m_acceleration = 20.0f; // slow
	this->m_turn_rate = 40.0f; //2 degrees per second
	m_selection_sound = "assets/sounds/Selection_Alien.ogg";

	m_aspects.push_back(new AspectPhysics2D(this));
}

Entity381Alien::~Entity381Alien(void) {
}
