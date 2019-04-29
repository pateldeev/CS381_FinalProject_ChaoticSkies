#include "AspectSubClasses/AspectRenderable.h"

#include "Entity381.h"

AspectRenderable::AspectRenderable(Entity381* entity) :
	Aspect(entity) {
}

AspectRenderable::~AspectRenderable(void) {
}

void AspectRenderable::Tick(float dt) {
	m_entity->GetOgreSceneNode()->setPosition(m_entity->GetPosition());

	float yaw = m_entity->GetHeading();
	float pitch = m_entity->GetPitch();
	float roll = m_entity->GetRoll();

	//std::cout<< "pitch: " << pitch << std::endl;
	m_entity->GetMeshOrientationsFixed(yaw, pitch, roll);

	Ogre::SceneNode* n = m_entity->GetOgreSceneNode();
	n->resetOrientation();
	n->yaw(Ogre::Degree(yaw));
	n->pitch(Ogre::Degree(pitch));
	n->roll(Ogre::Degree(roll));

	//std::cout << m_entity->GetRoll() << std::endl;
}
