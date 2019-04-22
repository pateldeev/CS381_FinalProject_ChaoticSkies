#include "Entity381SubClasses/Entity381Carrier.h"

Entity381Carrier::Entity381Carrier(Engine *engine, const std::string &mesh, int id, const Ogre::Vector3 &pos) :
	Entity381(engine, mesh, id, false, "assets/sounds/Selection_Carrier.ogg", pos) {

	m_speed_max = 40.0f;
	m_acceleration = 2.0f;
	m_turn_rate = 10.0f;
}

Entity381Carrier::~Entity381Carrier(void) {
}
