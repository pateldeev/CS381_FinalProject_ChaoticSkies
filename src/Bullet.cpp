#include "Bullet.h"

#include "Engine.h"

#include "GfxMgr.h"

#include "Utils.h"

unsigned int Bullet::id_generator = 0;

Bullet::Bullet(Engine* engine, const Ogre::Vector3 &pos, const Ogre::Vector3 &direction, float speed, float active_time) :
	m_engine(engine), m_id(id_generator++), m_name("Bullet_" + IntToString(m_id)), m_active_time_left(active_time), m_ogre_entity(nullptr),
	m_scene_node(nullptr), m_direction(speed * direction.normalisedCopy()) {
	m_ogre_entity = m_engine->GetGfxMgr()->GetOgreSceneManager()->createEntity("geosphere4500.mesh");

	Ogre::MaterialPtr m = m_ogre_entity->getSubEntity(0)->getMaterial()->clone(m_name + "_mat");
	m->setAmbient(1, 0, 0);
	m->setDiffuse(1, 0, 0, 0);
	m_ogre_entity->setMaterialName(m->getName());

	m_scene_node = m_engine->GetGfxMgr()->GetOgreSceneManager()->getRootSceneNode()->createChildSceneNode(m_name, pos);
	m_scene_node->setScale(Ogre::Vector3(0.006));
	m_scene_node->attachObject(m_ogre_entity);
}

Bullet::~Bullet(void) {
	m_scene_node->getParentSceneNode()->detachObject(m_ogre_entity);
		m_engine->GetGfxMgr()->GetOgreSceneManager()->destroySceneNode(m_scene_node->getName());
}

void Bullet::Tick(float dt) {
	m_active_time_left -= dt;
	if (m_active_time_left > 0)
		m_scene_node->translate(m_direction * dt, Ogre::Node::TransformSpace::TS_LOCAL);
}

void Bullet::Deactivate(void) {
	m_active_time_left = 0;
}

bool Bullet::HasCollidedWithInLastTick(const Ogre::Vector3 &pos, float radius) const {
	return (m_scene_node->_getDerivedPosition().distance(pos) < radius);
}

float Bullet::GetSpeed(void) const {
	return m_direction.length();
}

void Bullet::SetSpeed(float speed) {
	m_direction.normalise();
	m_direction *= speed;
}

unsigned int Bullet::GetId(void) const {
	return m_id;
}

std::string Bullet::GetName(void) const {
	return m_name;
}

Ogre::Vector3 Bullet::GetPosition(void) const {
	return m_scene_node->_getDerivedPosition();
}

bool Bullet::IsStillActive(void) const {
	return m_active_time_left > 0;
}

