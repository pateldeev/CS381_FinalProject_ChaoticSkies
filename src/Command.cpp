#include "Command.h"

#include "Entity381.h"

#include "EntityMgr.h"

Command::~Command(void) {
	Stop();
}

void Command::Init(void) {
	m_running = true;
}

void Command::Stop(void) {
	m_running = false;
}

bool Command::Done(void) const {
	return !m_running;
}

void Command::DrawBoundingBox(bool draw_all) const {
	m_parent->ShowBoundingBox();
#ifdef EDIT_BOUNDINGBOX_COLOR_EXPERIMENAL
	static_cast<BoudingBoxColorChangerExperimenatal*>(m_parent->GetOgreSceneNode())->MakeBoundingBoxBlack();
#endif
}

Command::Command(Entity381 *parent) :
	m_parent(parent), m_running(false) {
}
