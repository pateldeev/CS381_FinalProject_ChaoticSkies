#include "CommandSubClasses/CommandDie.h"

#include "Entity381.h"
#include "Utils.h"

CommandDie::CommandDie(Entity381 *parent) :
	Command(parent), m_time_left(5) {
}

CommandDie::~CommandDie(void) {
}

void CommandDie::Tick(float dt) {
	if (m_running) {
		m_time_left -= dt;

		if (m_time_left < 0 || m_parent->GetPosition().y < -25) {
			m_running = false;
			m_parent->PitchStop();
			m_parent->RollStop();
			m_parent->SetSpeedDesired(0);
			m_parent->Kill();
		}

		if (m_parent->GetPitch().valueDegrees() < 80)
			m_parent->PitchDown();
	}
}

void CommandDie::Init(void) {
	m_parent->SetSpeedDesired(m_parent->GetSpeedMax());
	m_parent->PitchDown();

	if (Rand01() < 0.5)
		m_parent->RollRight();
	else
		m_parent->RollLeft();

	Command::Init();
}
