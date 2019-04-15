#include "CommandSubClasses/CommandMoveTo.h"

#include "Entity381.h"

#include "EntityMgr.h"

float CommandMoveTo::acceptable_error = 12.f;

CommandMoveTo::CommandMoveTo(Entity381 *parent, const Ogre::Vector3 &position) :
	Command(parent), m_target(position), m_distance_to_target(0.f) {
}

CommandMoveTo::~CommandMoveTo(void) {
}

void CommandMoveTo::Tick(float dt) {
	m_distance_to_target = m_parent->m_position.distance(m_target);
	float speed_max = m_distance_to_target / 2;

	if (m_distance_to_target < acceptable_error) {
		m_parent->m_desired_speed = 0;
		m_running = false;
	} else {
		float heading_to_target = Ogre::Vector3::UNIT_X.angleBetween(m_target - m_parent->m_position).valueDegrees();
		if (m_target.z < m_parent->m_position.z)
			heading_to_target = 360 - heading_to_target;
		m_parent->m_desired_heading = heading_to_target;
		m_parent->m_desired_speed = std::min(m_parent->m_speed_max, speed_max);
	}
}

void CommandMoveTo::Init(void) {
	m_parent->m_desired_speed = m_parent->m_speed_max;
	m_distance_to_target = m_parent->m_position.distance(m_target);
	Command::Init();
}
