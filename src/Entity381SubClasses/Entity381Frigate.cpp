#include "Entity381SubClasses/Entity381Frigate.h"

Entity381Frigate::Entity381Frigate(Engine *engine, const std::string &mesh, const Ogre::Vector3 &pos) :
	Entity381(engine, mesh, false, "assets/sounds/Selection_Frigate.ogg", pos) {

	m_speed_max = 30;
	m_acceleration = 10;
	m_turn_rate = 20;
}

Entity381Frigate::~Entity381Frigate(void) {
}
