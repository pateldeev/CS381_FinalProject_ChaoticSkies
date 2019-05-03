#include "CommandSubClasses/CommandPatrol.h"

#include "Entity381.h"

CommandPatrol::CommandPatrol(Entity381* parent, const Ogre::Vector3 &center, const Ogre::Vector3 &extent) :
	Command(parent), m_center(center), m_extent(extent){
}

CommandPatrol::~CommandPatrol(void) {
}

void CommandPatrol::Tick(float dt) {
	if (m_running) {

	}
}

void CommandPatrol::Init(void) {
	m_parent->m_speed = m_parent->m_speed_desired =  m_parent->m_heading = m_parent->m_heading_desired = m_parent->m_pitch = m_parent->m_pitch_rate_current = m_parent->m_roll = m_parent->m_roll_rate_current = 0;

	Command::Init();
}
