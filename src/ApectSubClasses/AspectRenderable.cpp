#include "AspectSubClasses/AspectRenderable.h"

#include "Entity381.h"

AspectRenderable::AspectRenderable(Entity381* entity) :
	Aspect(entity) {
}

AspectRenderable::~AspectRenderable(void) {
}

void AspectRenderable::Tick(float dt) {
	m_entity->m_scene_node->setPosition(m_entity->m_position);

	float yaw = m_entity->m_heading;
	float pitch = m_entity->m_pitch;
	float roll = m_entity->m_roll;
	m_entity->FixMeshOrientation(yaw, pitch, roll);
	m_entity->m_scene_node->resetOrientation();
	m_entity->m_scene_node->yaw(Ogre::Degree(yaw));
	m_entity->m_scene_node->pitch(Ogre::Degree(pitch));
	m_entity->m_scene_node->roll(Ogre::Degree(roll));
}
