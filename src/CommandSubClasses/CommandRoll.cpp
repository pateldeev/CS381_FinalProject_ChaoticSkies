#include "CommandSubClasses/CommandRoll.h"

#include "Entity381.h"
#include "Utils.h"

CommandRoll::CommandRoll(Entity381* parent, bool roll_right, const Ogre::Degree &roll_rate) :
	Command(parent), m_roll_rate(roll_right ? roll_rate : -roll_rate), m_roll_target(0) {
}

CommandRoll::~CommandRoll(void) {
}

void CommandRoll::Tick(float dt) {
	if (m_running) {
		m_parent->Roll(m_roll_rate, true);

		if (Ogre::Math::Abs(m_parent->GetRoll() - m_roll_target).valueDegrees() < 5) {
			m_running = false;
			m_parent->RollStop();
		}
	}
}

void CommandRoll::Init(void) {
	Ogre::Degree roll_current = m_parent->GetRoll();
	m_roll_target = roll_current + Ogre::Degree((m_roll_rate.valueDegrees() > 0) ? 185 : -185);
	FixAngle(m_roll_target);
	m_parent->Roll(m_roll_rate, true);

	Command::Init();
}
