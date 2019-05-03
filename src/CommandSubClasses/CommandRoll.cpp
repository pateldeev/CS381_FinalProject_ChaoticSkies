#include "CommandSubClasses/CommandRoll.h"

#include "Entity381.h"

CommandRoll::CommandRoll(Entity381* parent, bool roll_right, float rool_rate) :
	Command(parent), m_roll_speed(roll_right ? -rool_rate : rool_rate), m_roll_target(0) {
}

CommandRoll::~CommandRoll(void) {
}

void CommandRoll::Tick(float dt) {
	if (m_running) {
		m_parent->m_roll_rate_current = m_roll_speed;

		if ((m_roll_speed > 0 && m_parent->GetRoll() > m_roll_target) || (m_roll_speed < 0 && m_parent->GetRoll() < m_roll_target)) {
			m_running = false;
			m_parent->m_roll_rate_current = 0;
		}
	}
}

void CommandRoll::Init(void) {
	m_roll_target = (m_roll_speed > 0) ? m_parent->GetRoll() + 360 : m_parent->GetRoll() - 360;
	m_parent->m_roll_rate_current = m_roll_speed;
	Command::Init();
}
