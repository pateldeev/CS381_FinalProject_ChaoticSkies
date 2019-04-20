#include "CommandSubClasses/CommandFollow.h"

#include "Entity381.h"

#include "EntityMgr.h"

float CommandFollow::max_follow_distance = 50.f;

CommandFollow::CommandFollow(Entity381 *parent, const Entity381 *target) :
	CommandMoveTo(parent, target->GetPosition()), m_following(target) {
}

CommandFollow::~CommandFollow(void) {
}

void CommandFollow::Tick(float dt) {
	m_parent->SetSpeedDesired(m_parent->GetSpeedMax());
	CommandMoveTo::Tick(dt);

	if (m_parent->GetPosition().distance(m_target) < CommandFollow::max_follow_distance)
		m_parent->SetSpeedDesired(m_following->GetSpeed());

	if (m_parent->GetSpeed() > 1)
		m_target = m_following->GetPosition() + (m_following->GetVelocity() * dt * (m_distance_to_target / m_parent->GetSpeed()));
	else
		m_target = m_following->GetPosition();

	//m_target = m_following->GetPosition();

	m_running = true; //prevent command from ending once target reached
}

void CommandFollow::DrawBoundingBox(bool draw_all) const {
	Command::DrawBoundingBox();
	if (draw_all) {
		m_following->ShowBoundingBox();
#ifdef EDIT_BOUNDINGBOX_COLOR_EXPERIMENAL
		static_cast<BoudingBoxColorChangerExperimenatal*>(const_cast<Ogre::SceneNode*>(m_following->GetOgreSceneNode()))->MakeBoundingBoxOrange();
#endif
	}
}
