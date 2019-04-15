#include "AspectSubClasses/AspectPhysics3D.h"

#include "Entity381.h"

#include "Utils.h"

AspectPhysics3D::AspectPhysics3D(Entity381* entity) :
	Aspect(entity) {
}

AspectPhysics3D::~AspectPhysics3D(void) {
}

void AspectPhysics3D::Tick(float dt) {
	m_entity->m_speed = ProgressValueTowards(m_entity->m_speed, m_entity->m_desired_speed, m_entity->m_acceleration * dt);
	m_entity->m_speed = Clamp(m_entity->m_speed_min, m_entity->m_speed_max, m_entity->m_speed);

	//std::cout << "P:" << m_entity->m_pitch << "|" << m_entity->m_desired_pitch << std::endl << "R:" << m_entity->m_roll << "|" << m_entity->m_desired_roll << std::endl;

	m_entity->m_desired_pitch = FixAngle(m_entity->m_desired_pitch);
	m_entity->m_pitch = ProgressAngleTowardsDesired(m_entity->m_pitch, m_entity->m_desired_pitch, 10 * dt);
	m_entity->m_desired_roll = FixAngle(m_entity->m_desired_roll);
	m_entity->m_roll = ProgressAngleTowardsDesired(m_entity->m_roll, m_entity->m_desired_roll, 10 * dt);

	m_entity->m_desired_heading += Ogre::Math::Sin(Ogre::Degree(m_entity->m_roll)) * dt * m_entity->m_turn_rate;

	m_entity->m_desired_heading = FixAngle(m_entity->m_desired_heading);
	m_entity->m_heading = ProgressAngleTowardsDesired(m_entity->m_heading, m_entity->m_desired_heading, m_entity->m_turn_rate * dt);

	Ogre::Vector3 v(Ogre::Math::Cos(Ogre::Degree(m_entity->m_heading)), 0.f, -Ogre::Math::Sin(Ogre::Degree(m_entity->m_heading)));
	float vel_y = Ogre::Math::Sin(Ogre::Degree(m_entity->m_pitch)) * m_entity->m_speed;
	float v_len = Ogre::Math::Cos(Ogre::Degree(m_entity->m_pitch)) * m_entity->m_speed;
	m_entity->m_velocity = v_len * v.normalisedCopy();
	m_entity->m_velocity.y = vel_y;

	m_entity->m_position += m_entity->m_velocity * dt;
}
