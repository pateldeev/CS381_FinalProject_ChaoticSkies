#include "Entity381SubClasses/Entity381Frigate.h"

Entity381Frigate::Entity381Frigate(Engine *engine, const std::string &mesh, int id, const Ogre::Vector3 &pos) :
	Entity381(engine, mesh, id, false, "assets/sounds/Selection_Frigate.ogg", pos) {

	m_speed_max = 30.0f;
	m_acceleration = 10.0f;
	m_turn_rate = 20.0f;
}

Entity381Frigate::~Entity381Frigate(void) {
}
