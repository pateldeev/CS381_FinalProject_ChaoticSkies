#include "AspectSubClasses/AspectPhysics2D.h"

#include "Entity381.h"

#include "Utils.h"

AspectPhysics2D::AspectPhysics2D(Entity381* entity) :
	Aspect(entity) {
}

AspectPhysics2D::~AspectPhysics2D(void) {
}

void AspectPhysics2D::Tick(float dt) {
	m_entity->m_speed = ProgressValueTowards(m_entity->m_speed, m_entity->m_desired_speed, m_entity->m_acceleration * dt);
	m_entity->m_speed = Clamp(m_entity->m_speed_min, m_entity->m_speed_max, m_entity->m_speed);

	m_entity->m_desired_heading = FixAngle(m_entity->m_desired_heading);
	m_entity->m_heading = ProgressAngleTowardsDesired(m_entity->m_heading, m_entity->m_desired_heading, m_entity->m_turn_rate * dt);

	m_entity->m_velocity.y = 0.f; //since in 2D only
	m_entity->m_velocity.x = Ogre::Math::Cos(Ogre::Degree(m_entity->m_heading)) * m_entity->m_speed; //adjacent/hyp
	m_entity->m_velocity.z = -Ogre::Math::Sin(Ogre::Degree(m_entity->m_heading)) * m_entity->m_speed; //opposite/hyp

	m_entity->m_position += m_entity->m_velocity * dt;
}
