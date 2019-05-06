#include "Entity381SubClasses/Entity381SpeedBoat.h"

Entity381SpeedBoat::Entity381SpeedBoat(Engine *engine, const std::string &mesh, const Ogre::Vector3 &pos) :
	Entity381(engine, mesh, false, "assets/sounds/Selection_Speedboat.ogg", pos) {

	m_speed_max = 60;
	m_acceleration = 10;
	m_turn_rate = 30;
}

Entity381SpeedBoat::~Entity381SpeedBoat(void) {
}
