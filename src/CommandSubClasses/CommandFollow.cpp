#include "CommandSubClasses/CommandFollow.h"

#include "Entity381.h"

#include "EntityMgr.h"

float CommandFollow::max_follow_distance = 50.f;

CommandFollow::CommandFollow(Entity381 *parent, const Entity381 *target) :
	CommandMoveTo(parent, target->m_position), m_following(target) {
}

CommandFollow::~CommandFollow(void) {
}

void CommandFollow::Tick(float dt) {
	m_parent->m_desired_speed = m_parent->m_speed_max;
	CommandMoveTo::Tick(dt);

	if (m_parent->m_position.distance(m_target) < CommandFollow::max_follow_distance)
		m_parent->m_desired_speed = m_following->m_speed;

	if (m_parent->m_speed > 1)
		m_target = m_following->m_position + (m_following->m_velocity * dt * (m_distance_to_target / m_parent->m_speed));
	else
		m_target = m_following->m_position;

	m_target = m_following->m_position;

	m_running = true; //prevent command from ending once target reached
}

void CommandFollow::DrawBoundingBox(bool draw_all) const {
	Command::DrawBoundingBox();
	if (draw_all) {
		m_following->m_scene_node->showBoundingBox(true);
#ifdef EDIT_BOUNDINGBOX_COLOR_EXPERIMENAL
		static_cast<BoudingBoxColorChangerExperimenatal*>(m_following->m_scene_node)->MakeBoundingBoxOrange();
#endif
	}
}
