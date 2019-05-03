#include "CommandSubClasses/CommandPitch.h"

#include "Entity381.h"

CommandPitch::CommandPitch(Entity381* parent, bool pitch_up, float pitch_rate) :
	Command(parent), m_pitch_speed(pitch_up ? -pitch_rate : pitch_rate), m_pitch_target(0) {
}

CommandPitch::~CommandPitch(void) {
}

void CommandPitch::Tick(float dt) {
	if (m_running) {
		m_parent->m_pitch_rate_current = m_pitch_speed;

		if ((m_pitch_speed > 0 && m_parent->GetPitch() > m_pitch_target) || (m_pitch_speed < 0 && m_parent->GetPitch() < m_pitch_target)) {
			m_running = false;
			m_parent->m_pitch_rate_current = 0;
		}
	}
}

void CommandPitch::Init(void) {
	m_pitch_target = (m_pitch_speed > 0) ? m_parent->GetPitch() + 180 : m_parent->GetPitch() - 180;
	m_parent->m_pitch_rate_current = m_pitch_speed;
	Command::Init();
}
