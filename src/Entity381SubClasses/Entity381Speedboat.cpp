#include "Entity381SubClasses/Entity381SpeedBoat.h"

Entity381SpeedBoat::Entity381SpeedBoat(Engine *engine, const std::string &mesh, int id, const Ogre::Vector3 &pos) :
	Entity381(engine, mesh, id, false, "assets/sounds/Selection_Speedboat.ogg", pos) {

	m_speed_max = 60.0f;
	m_acceleration = 10.0f;
	m_turn_rate = 30.0f;
}

Entity381SpeedBoat::~Entity381SpeedBoat(void) {
}
