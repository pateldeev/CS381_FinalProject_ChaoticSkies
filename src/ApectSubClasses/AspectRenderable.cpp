#include "AspectSubClasses/AspectRenderable.h"

#include "Entity381.h"

AspectRenderable::AspectRenderable(Entity381* entity) :
	Aspect(entity) {
}

AspectRenderable::~AspectRenderable(void) {
}

void AspectRenderable::Tick(float dt) {
	m_entity->GetOgreSceneNode()->setPosition(m_entity->GetPosition());

	Ogre::SceneNode* n = m_entity->GetOgreSceneNode();
	n->resetOrientation();
	if (!m_entity->Is3D())
		n->yaw(Ogre::Degree(m_entity->GetHeading()));
	else
		n->rotate(m_entity->GetRotationWorld());
}
