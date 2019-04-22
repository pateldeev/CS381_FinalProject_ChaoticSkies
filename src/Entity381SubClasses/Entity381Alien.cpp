#include "Entity381SubClasses/Entity381Alien.h"

Entity381Alien::Entity381Alien(Engine *engine, const std::string &mesh, int id, const Ogre::Vector3 &pos) :
	Entity381(engine, mesh, id, false, "assets/sounds/Selection_Alien.ogg", pos) {

	m_speed_max = 100.0f;
	m_acceleration = 20.0f;
	m_turn_rate = 40.0f;
}

Entity381Alien::~Entity381Alien(void) {
}
