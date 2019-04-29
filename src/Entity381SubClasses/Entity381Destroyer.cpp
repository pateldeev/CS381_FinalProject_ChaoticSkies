#include "Entity381SubClasses/Entity381Destroyer.h"

Entity381Destroyer::Entity381Destroyer(Engine *engine, const std::string &mesh, const Ogre::Vector3 &pos) :
	Entity381(engine, mesh, false, "assets/sounds/Selection_Destroyer.ogg", pos) {

	m_speed_max = 32.0f;
	m_acceleration = 10.0f;
	m_turn_rate = 20.0f;
}

Entity381Destroyer::~Entity381Destroyer(void) {
}
