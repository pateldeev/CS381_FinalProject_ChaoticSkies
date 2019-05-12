#include "AspectSubClasses/AspectUnitAI.h"

#include "Entity381.h"

AspectUnitAI::AspectUnitAI(Entity381* entity) :
	Aspect(entity) {
}

AspectUnitAI::~AspectUnitAI(void) {
	RemoveAllCommands();
}

void AspectUnitAI::Tick(float dt) {
	if (!m_commands.empty()) {
		m_commands.front()->Tick(dt);

		if (m_commands.front()->Done()) {
			delete m_commands.front();
			m_commands.pop();
			if (!m_commands.empty())
				m_commands.front()->Init();
		}
	}
}

void AspectUnitAI::SetCommand(Command *c) {
	RemoveAllCommands();
	m_commands.push(c);
	c->Init();
}

void AspectUnitAI::AddCommand(Command *c) {
	m_commands.push(c);
	if (m_commands.front() == c)
		c->Init();
}

unsigned int AspectUnitAI::GetNumCommands(void) const {
	return m_commands.empty() ? 0 : m_commands.size();
}

void AspectUnitAI::RemoveAllCommands(void) {
	while (!m_commands.empty()) {
		m_commands.front()->Stop();
		delete m_commands.front();
		m_commands.pop();
	}
}

void AspectUnitAI::DrawBoundingBox(bool draw_all) const {
	if (!m_commands.empty())
		m_commands.front()->DrawBoundingBox(draw_all);
}
