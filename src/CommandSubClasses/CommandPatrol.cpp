#include "CommandSubClasses/CommandPatrol.h"

#include "Entity381.h"

#include "Utils.h"

#include "UIMgr.h"

CommandPatrol::CommandPatrol(Entity381* parent, Entity381* main_plane, const Ogre::Vector3 &center, const Ogre::Vector3 &extent) :
	Command(parent), m_center(center), m_extent(extent), m_target(0.f), m_plane(main_plane), m_firing_cooldown(2) {
}

CommandPatrol::~CommandPatrol(void) {
#if SHOW_TARGET
	delete m_b;
#endif
	for (Bullet* b : m_bullets)
		delete b;
}

void CommandPatrol::Tick(float dt) {
	if (m_running) {
		UpdateTargetIfClose();

		Ogre::Plane p(Ogre::Vector3::UNIT_Y, 0);
		Ogre::Vector3 v1 = p.projectVector(m_parent->GetDirection());
		Ogre::Vector3 v2 = p.projectVector(m_target - m_parent->GetPosition());
		Ogre::Degree yaw_to_target = v1.getRotationTo(v2).getYaw();
		Ogre::Degree yaw_rate_target = yaw_to_target / 2;
		Ogre::Degree yaw_rate_current = m_parent->GetYawRate();

		//std::cout << yaw_to_target << "|" << yaw_rate_current << std::endl;

		m_parent->Roll(yaw_rate_current - yaw_rate_target);

		float y_dist_to_target = m_target.y - m_parent->GetPosition().y;

		//std::cout << y_dist_to_target << "|" << m_parent->GetVelocity() << std::endl;

		if (y_dist_to_target < -2) {
			if (m_parent->GetVelocity().y < y_dist_to_target)
				m_parent->PitchUp();
			else
				m_parent->PitchDown();
		} else if (y_dist_to_target > 2) {
			if (m_parent->GetVelocity().y > y_dist_to_target)
				m_parent->PitchDown();
			else
				m_parent->PitchUp();
		} else {
			m_parent->PitchStop();
		}

		EnsureRollAndPitchReasonable();

		UpdateBulletsAndFiring(dt);
	}
}

void CommandPatrol::Init(void) {
	m_parent->m_speed = m_parent->m_speed_desired = 0;
	m_parent->m_yaw_rate = m_parent->m_pitch_rate = m_parent->m_roll_rate = Ogre::Degree(0);
	m_parent->SetSpeedDesired(55);

	UpdateTarget(m_center);

	Command::Init();
}

void CommandPatrol::EnsureRollAndPitchReasonable(void) {
	//if ((m_parent->GetRoll().valueDegrees() > 42 && m_parent->GetRollRate().valueDegrees() > 0) || (m_parent->GetRoll().valueDegrees() < -42 && m_parent->GetRollRate().valueDegrees() < 0))
	//m_parent->RollStop();

	if ((m_parent->GetPitch().valueDegrees() > 45 && m_parent->GetPitchRate().valueDegrees() > 0) || (m_parent->GetPitch().valueDegrees() < -45 && m_parent->GetPitchRate().valueDegrees() < 0))
		m_parent->PitchStop();

	if (m_parent->GetPosition().y <= 15 && m_parent->GetPitchRate().valueDegrees() > 0)
		m_parent->PitchUp();
}

void CommandPatrol::UpdateTargetIfClose(void) {
	if (m_parent->GetPosition().distance(m_target) < 10) {
		Ogre::Vector3 v(Rand01() - 0.5, Rand01() - 0.5, Rand01() - 0.5);
		Ogre::Vector3 v_change(m_extent.x * v.x, m_extent.y * v.y, m_extent.z * v.z);

		//std::cout << "CLOSE: " << v_change << std::endl;

		UpdateTarget(m_center + 2 * v_change);
		//UpdateTarget(m_plane->GetPosition());
		//UpdateTarget(Ogre::Vector3(700, 10, -250));
	}
}

void CommandPatrol::UpdateTarget(const Ogre::Vector3 &target) {
	m_target = target;
	if (m_target.y < 1)
		m_target.y = 1;

	//std::cout << "Target Updated: " << m_target << std::endl;

#if SHOW_TARGET
	delete m_b;
	m_b = new Bullet(m_parent->m_engine, m_target, m_target);
#endif
}

void CommandPatrol::UpdateBulletsAndFiring(float dt) {
	for (std::list<Bullet*>::iterator b = m_bullets.begin(); b != m_bullets.end(); ++b) {
		if (!((*b)->IsStillActive())) {
			delete *b;
			b = m_bullets.erase(b);
		} else {
			(*b)->Tick(dt);
			//check for collision with plane
			if ((*b)->HasCollidedWithInLastTick(m_plane->GetPosition())) {
				m_plane->m_engine->GetUIMgr()->SetHealthBarPercentage(-10, true);
				(*b)->Deactivate();
			}
		}
	}

	m_firing_cooldown -= dt;
	if (m_firing_cooldown < 0) {
		m_firing_cooldown = 2;
		Ogre::Vector3 d = m_parent->GetDirection();
		m_bullets.push_front(new Bullet(m_parent->m_engine, m_parent->GetPosition() + 15 * d, d));
	}
}
