#include "CommandSubClasses/CommandPatrol.h"

#include "Entity381.h"

#include "Utils.h"

CommandPatrol::CommandPatrol(Entity381* parent, const Ogre::Vector3 &center, const Ogre::Vector3 &extent) :
	Command(parent), m_center(center), m_extent(extent), m_yaw_direction(0), m_pitch_direction(0), m_roll_direction(0) {
}

CommandPatrol::~CommandPatrol(void) {
}

void CommandPatrol::Tick(float dt) {
	if (m_running) {
		UpdateYawDirection();
		UpdatePitchDirection();
		UpdateRollDirection();

		EnsureRollAndPitchReasonable();

		if (m_yaw_direction == 1)
			m_parent->YawLeft();
		else if (m_yaw_direction == -1)
			m_parent->YawRight();
		else
			m_parent->YawStop();

		if (m_pitch_direction == 1)
			m_parent->PitchUp();
		else if (m_pitch_direction == -1)
			m_parent->PitchDown();
		else
			m_parent->PitchStop();

		if (m_roll_direction == 1)
			m_parent->RollRight();
		else if (m_roll_direction == -1)
			m_parent->RollLeft();
		else
			m_parent->RollStop();
	}
}

void CommandPatrol::Init(void) {
	m_parent->m_speed = m_parent->m_speed_desired = 0;
	m_parent->m_yaw_rate = m_parent->m_pitch_rate = m_parent->m_roll_rate = Ogre::Degree(0);
	m_parent->SetSpeedDesired(50);

	Command::Init();
}

void CommandPatrol::EnsureRollAndPitchReasonable(void) {
	if (std::abs(m_parent->GetRoll().valueDegrees()) > 40)
		m_roll_direction = 0;

	if (std::abs(m_parent->GetPitch().valueDegrees()) > 40)
		m_pitch_direction = 0;

	if (m_parent->GetPosition().y <= 25)
		m_pitch_direction = 1;
}

void CommandPatrol::UpdateYawDirection(void) {
	static const float start_chance = 0.004;
	static const float stop_chance = 0.001;
	static const float reverse_chance = 0.00005;

	float f = Rand01();
	if (m_yaw_direction == 0) {
		if (f < start_chance / 2)
			m_yaw_direction = 1;
		else if (f < start_chance)
			m_yaw_direction = -1;
	} else {
		if (f < reverse_chance)
			m_yaw_direction *= -1;
		else if (f < stop_chance)
			m_yaw_direction = 0;
	}
}

void CommandPatrol::UpdatePitchDirection(void) {
	static const float start_chance = 0.001;
	static const float stop_chance = 0.002;
	static const float reverse_chance = 0.00005;

	float f = Rand01();
	if (m_pitch_direction == 0) {
		if (f < start_chance / 2)
			m_pitch_direction = 1;
		else if (f < start_chance)
			m_pitch_direction = -1;
	} else {
		if (f < reverse_chance)
			m_pitch_direction *= -1;
		else if (f < stop_chance)
			m_pitch_direction = 0;
	}
}

void CommandPatrol::UpdateRollDirection(void) {
	static const float start_chance = 0.0008;
	static const float stop_chance = 0.0025;
	static const float reverse_chance = 0.00005;

	float f = Rand01();
	if (m_roll_direction == 0) {
		if (f < start_chance / 2)
			m_roll_direction = 1;
		else if (f < start_chance)
			m_roll_direction = -1;
	} else {
		if (f < reverse_chance)
			m_roll_direction *= -1;
		else if (f < stop_chance)
			m_roll_direction = 0;
	}
}
