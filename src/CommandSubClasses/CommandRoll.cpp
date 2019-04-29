#include "CommandSubClasses/CommandRoll.h"

#include "Entity381.h"

CommandRoll::CommandRoll(Entity381 *parent, bool roll_right) :
	Command(parent), m_roll_rate(roll_right ? -100 : 100), m_roll_target(0) {
}

CommandRoll::~CommandRoll(void) {
}

void CommandRoll::Tick(float dt) {
	if (m_running) {
		std::cout << m_parent->GetRoll() << "|Target:" << m_roll_target << " @ " << m_roll_rate << std::endl;
		if ((m_roll_rate < 0 && m_parent->GetRoll() < m_roll_target) || (m_roll_rate > 0 && m_parent->GetRoll() > m_roll_target)) {
			m_running = false;
			m_parent->m_roll_rate_current = 0;
			std::cout << "Done: " << m_parent->GetRoll() << std::endl;
		} else {
			m_parent->m_roll_rate_current = m_roll_target;
		}
	}
}

void CommandRoll::Init(void) {
	m_parent->m_roll_rate_current = m_roll_rate;
	if (m_roll_rate > 0)
		m_roll_target = m_parent->GetRoll() + 360;
	else
		m_roll_target = m_parent->GetRoll() - 360;
	std::cout << "INIT: " << m_parent->GetRoll() << "|Target:" << m_roll_target << " @ " << m_roll_rate << std::endl;
	Command::Init();
}
