#include "CommandSubClasses/CommandIntercept.h"

#include "Entity381.h"

#include "EntityMgr.h"

CommandIntercept::CommandIntercept(Entity381 *parent, const Entity381 *target) :
	CommandMoveTo(parent, target->GetPosition()), m_intercept_target(target) {
}

CommandIntercept::~CommandIntercept(void) {
}

void CommandIntercept::Tick(float dt) {
	CommandMoveTo::Tick(dt);

	if (m_parent->GetSpeed() > 1)
		m_target = m_intercept_target->GetPosition() + (m_intercept_target->GetVelocity() * dt * (m_distance_to_target / m_parent->GetSpeed()));
	else
		m_target = m_intercept_target->GetPosition();

	m_parent->SetSpeedDesired(m_parent->GetSpeedMax());
	m_running = true; //prevent command from ending once target reached
}

void CommandIntercept::DrawBoundingBox(bool draw_all) const {
	Command::DrawBoundingBox();
	if (draw_all) {
		m_intercept_target->ShowBoundingBox();
#ifdef EDIT_BOUNDINGBOX_COLOR_EXPERIMENAL
		static_cast<BoudingBoxColorChangerExperimenatal*>(const_cast<Ogre::SceneNode*>(m_intercept_target->GetOgreSceneNode()))->MakeBoundingBoxRed();
#endif
	}
}
