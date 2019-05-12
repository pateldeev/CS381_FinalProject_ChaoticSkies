#include "CommandSubClasses/CommandPatrol.h"

#include "Entity381.h"

#include "Utils.h"

#include "UIMgr.h"
#include "GameMgr.h"

CommandPatrol::CommandPatrol(Entity381* parent, const Entity381* main_plane, const Ogre::Vector3 &center, const Ogre::Vector3 &extent, int patrol_speed) :
	Command(parent), m_center(center), m_extent(extent), m_patrol_speed(patrol_speed), m_target(0.f), m_new_target_time(20), m_plane(main_plane), m_is_following(false), m_firing_cooldown(2) {
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
		m_new_target_time -= dt;
		UpdateTargetIfNecessary();

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
	m_parent->SetSpeedDesired(m_patrol_speed);

	m_parent->HideBoundingBox();

	UpdateTarget(m_center);

	Command::Init();
}

void CommandPatrol::EnsureRollAndPitchReasonable(void) {
	//if ((m_parent->GetRoll().valueDegrees() > 42 && m_parent->GetRollRate().valueDegrees() > 0) || (m_parent->GetRoll().valueDegrees() < -42 && m_parent->GetRollRate().valueDegrees() < 0))
	//m_parent->RollStop();

	if ((m_parent->GetPitch().valueDegrees() > 45 && m_parent->GetPitchRate().valueDegrees() > 0) || (m_parent->GetPitch().valueDegrees() < -45 && m_parent->GetPitchRate().valueDegrees() < 0))
		m_parent->PitchStop();

	if (m_parent->GetPosition().y <= 20 && m_parent->GetPitchRate().valueDegrees() > 0)
		m_parent->PitchUp();

	if (m_parent->GetPosition().y >= 200 && m_parent->GetPitchRate().valueDegrees() < 0)
		m_parent->PitchDown();
}

void CommandPatrol::UpdateTargetIfNecessary(void) {
	if (m_is_following) {
		if (!IsPlaneInPatrolZone()) {
			m_is_following = false;
			m_parent->m_engine->GetGameMgr()->RemoveEnemyFollowing();
			UpdateTargetIfNecessary();
			m_parent->HideBoundingBox();
			return;
		}

		if (m_new_target_time < 0 || m_parent->GetPosition().distance(m_target) < 10) {
			m_new_target_time = 2;

			Ogre::Vector3 plane_pos = m_plane->GetPosition();
			float intercept_time = m_parent->GetPosition().distance(plane_pos) / m_patrol_speed;
			Ogre::Vector3 target_new = plane_pos + m_plane->GetVelocity() * intercept_time;

			UpdateTarget(target_new);
			//UpdateTarget(m_plane->GetPosition());
		}
	} else {
		if (IsPlaneInPatrolZone()) {
			m_is_following = true;
			m_parent->m_engine->GetGameMgr()->AddEnemyFollowing();
			UpdateTargetIfNecessary();
			m_parent->ShowBoundingBox();
			return;
		}

		if (m_new_target_time < 0 || m_parent->GetPosition().distance(m_target) < 10) {
			m_new_target_time = 10;

			Ogre::Vector3 v_rand(Rand01() - 0.5, Rand01() - 0.5, Rand01() - 0.5);
			Ogre::Vector3 target_new = m_center + 2 * m_extent * v_rand;
			UpdateTarget(target_new);
			//UpdateTarget(Ogre::Vector3(700, 10, -250));
		}
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

bool CommandPatrol::IsPlaneInPatrolZone(void) const {
	Ogre::Vector3 plane_pos = m_plane->GetPosition();
	return (plane_pos.x < m_center.x + m_extent.x && plane_pos.x > m_center.x - m_extent.x && plane_pos.y < m_center.y + m_extent.y && plane_pos.y > m_center.y - m_extent.y && plane_pos.z < m_center.z + m_extent.z
		&& plane_pos.z > m_center.z - m_extent.z);

}

void CommandPatrol::UpdateBulletsAndFiring(float dt) {
	for (std::list<Bullet*>::iterator b = m_bullets.begin(); b != m_bullets.end(); ++b) {
		if (!((*b)->IsStillActive())) {
			delete *b;
			b = m_bullets.erase(b);
		} else {
			(*b)->Tick(dt);
			//check for collision with plane
			if ((*b)->HasCollidedWithInLastTick(m_plane->GetPosition(), 45)) {
				m_plane->m_engine->GetUIMgr()->SetHealthBarPercentage(-20, true);
				m_plane->m_engine->GetGameMgr()->MakeExplosionSound();
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
