#include "CommandSubClasses/CommandDie.h"

#include "Entity381.h"

CommandDie::CommandDie(Entity381 *parent) :
	Command(parent), m_pitch_target(0) {
}

CommandDie::~CommandDie(void) {
}

void CommandDie::Tick(float dt) {
	if (m_running) {
		if(m_parent->GetPosition().y < -25){
			m_running = false;
			m_parent->PitchStop();
			m_parent->RollStop();
			m_parent->SetSpeedDesired(0);
			m_parent->Kill();
		}

		if(m_pitch_target >  m_parent->GetPitch())
			m_parent->PitchUp();
		else
			m_parent->PitchDown();
	}
}

void CommandDie::Init(void) {
	float pitch =  m_parent->GetPitch();
	int looped =pitch/360;

	float target_1 = -90 + 360*looped;
	float target_2 = target_1+360;
	if(std::abs(pitch - target_1) < std::abs(pitch - target_2))
		m_pitch_target = target_1;
	else
		m_pitch_target = target_2;

	m_parent->SetSpeedDesired(50);
	m_parent->RollRight();
	//std::cout << "Command Die: " << "Pitch: " << m_parent->GetPitch() << " | Target: " << m_pitch_target << std::endl;
	Command::Init();
}
