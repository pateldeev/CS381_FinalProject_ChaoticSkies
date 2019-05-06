#include "CommandSubClasses/CommandPitch.h"

#include "Entity381.h"
#include "Utils.h"

CommandPitch::CommandPitch(Entity381* parent, bool pitch_up, float pitch_rate) :
	Command(parent), m_pitch_rate(pitch_up ? pitch_rate : -pitch_rate), m_pitch_target(0) {
}

CommandPitch::~CommandPitch(void) {
}

void CommandPitch::Tick(float dt) {
	if (m_running) {
		m_parent->Pitch(m_pitch_rate, true);

		if (Ogre::Math::Abs(m_parent->GetPitch() - m_pitch_target).valueDegrees() < 1) {
			m_running = false;
			m_parent->PitchStop();
		}
	}
}

void CommandPitch::Init(void) {
	Ogre::Degree pitch_current = m_parent->GetPitch();
	m_pitch_target = pitch_current + Ogre::Degree((m_pitch_rate.valueDegrees() > 0) ? 180 : -180);
	FixAngle(m_pitch_target);
	m_parent->Pitch(m_pitch_target, true);

	Command::Init();
}
