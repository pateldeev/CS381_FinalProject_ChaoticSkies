#include "Entity381SubClasses/Entity381Carrier.h"

Entity381Carrier::Entity381Carrier(Engine *engine, const std::string &mesh, const Ogre::Vector3 &pos) :
	Entity381(engine, mesh, false, "assets/sounds/Selection_Carrier.ogg", pos) {

	m_speed_max = 40;
	m_acceleration = 2;
	m_turn_rate = 10;
}

Entity381Carrier::~Entity381Carrier(void) {
}
