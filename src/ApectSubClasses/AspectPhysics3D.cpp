#include "AspectSubClasses/AspectPhysics3D.h"

#include "Entity381.h"

#include "Utils.h"

AspectPhysics3D::AspectPhysics3D(Entity381* entity) :
	Aspect(entity) {
}

AspectPhysics3D::~AspectPhysics3D(void) {
}

void AspectPhysics3D::Tick(float dt) {
	m_entity->m_speed = ProgressValueTowards(m_entity->m_speed, m_entity->m_speed_desired, m_entity->m_acceleration * dt);
	m_entity->m_speed = Clamp(m_entity->m_speed_min, m_entity->m_speed_max, m_entity->m_speed);

	m_entity->m_yaw_rate = -m_entity->m_yaw_rate_max * Ogre::Math::Sin(m_entity->GetRoll()); // * Ogre::Math::Cos(m_entity->GetPitch());

	m_entity->UpdateRotation(dt);

	m_entity->m_velocity = m_entity->m_speed * m_entity->GetDirection();
	m_entity->m_position += m_entity->m_velocity * dt;
}
