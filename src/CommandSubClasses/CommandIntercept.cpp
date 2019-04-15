#include "CommandSubClasses/CommandIntercept.h"

#include "Entity381.h"

#include "EntityMgr.h"

CommandIntercept::CommandIntercept(Entity381 *parent, const Entity381 *target) :
	CommandMoveTo(parent, target->m_position), m_intercept_target(target) {
}

CommandIntercept::~CommandIntercept(void) {
}

void CommandIntercept::Tick(float dt) {
	CommandMoveTo::Tick(dt);

	if (m_parent->m_speed > 1)
		m_target = m_intercept_target->m_position + (m_intercept_target->m_velocity * dt * (m_distance_to_target / m_parent->m_speed));
	else
		m_target = m_intercept_target->m_position;

	m_parent->m_desired_speed = m_parent->m_speed_max;
	m_running = true; //prevent command from ending once target reached
}

void CommandIntercept::DrawBoundingBox(bool draw_all) const {
	Command::DrawBoundingBox();
	if (draw_all) {
		m_intercept_target->m_scene_node->showBoundingBox(true);
#ifdef EDIT_BOUNDINGBOX_COLOR_EXPERIMENAL
		static_cast<BoudingBoxColorChangerExperimenatal*>(m_intercept_target->m_scene_node)->MakeBoundingBoxRed();
#endif
	}
}
