#include "Entity381SubClasses/Entity381Alien.h"

Entity381Alien::Entity381Alien(Engine *engine, const std::string &mesh, const Ogre::Vector3 &pos) :
	Entity381(engine, mesh, false, "assets/sounds/Selection_Alien.ogg", pos) {

	m_speed_max = 100;
	m_acceleration = 20;
	m_turn_rate = 40;
}

Entity381Alien::~Entity381Alien(void) {
}
