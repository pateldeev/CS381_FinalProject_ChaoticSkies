#include "Entity381SubClasses/Entity381Destroyer.h"

Entity381Destroyer::Entity381Destroyer(Engine *engine, const std::string &meshfilename, int identity, const Ogre::Vector3 &pos) :
	Entity381(engine, meshfilename, identity, pos) {
	this->m_speed_min = 0;
	this->m_speed_max = 32.0f; //meters per second...
	this->m_acceleration = 10.0f; // fast
	this->m_turn_rate = 20.0f; //4 degrees per second
	m_selection_sound = "assets/sounds/Selection_Destroyer.ogg";

	m_aspects.push_back(new AspectPhysics2D(this));
}

Entity381Destroyer::~Entity381Destroyer(void) {
}
