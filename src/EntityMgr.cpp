#include "Engine.h"

#include "GfxMgr.h"
#include "InputMgr.h"
#include "SoundMgr.h"
#include "GameMgr.h"
#include "EntityMgr.h"

EntityMgr::EntityMgr(Engine* engine) :
	Mgr(engine), m_entity_selected(nullptr), m_entity_selected_index(-1) {
}

EntityMgr::~EntityMgr(void) {
	Stop();
}

void EntityMgr::Init(void) {

}
void EntityMgr::LoadLevel(void) {
	for (Entity381* e : m_entities)
		m_engine->GetSoundMgr()->LoadAudio(e->m_selection_sound, &e->m_audio_id, false);
}

void EntityMgr::Tick(float dt) {
	for (Entity381* e : m_entities)
		e->Tick(dt);

	if (m_entity_selected) {
		m_entity_selected->m_aspect_ai->DrawBoundingBox(true);
		m_entity_selected->m_scene_node->showBoundingBox(true);
#ifdef EDIT_BOUNDINGBOX_COLOR_EXPERIMENAL
		static_cast<BoudingBoxColorChangerExperimenatal*>(m_entity_selected->m_scene_node)->MakeBoundingBoxGreen();
#endif
	}

	for (unsigned int i : m_entities_selected) {
		m_entities[i]->m_scene_node->showBoundingBox(true);
#ifdef EDIT_BOUNDINGBOX_COLOR_EXPERIMENAL
		static_cast<BoudingBoxColorChangerExperimenatal*>(m_entities[i]->m_scene_node)->MakeBoundingBoxBlue();
#endif

	}
}

void EntityMgr::Stop(void) {

}

Entity381* EntityMgr::CreateEntityOfTypeAtPosition(Entity381Types entType, const Ogre::Vector3 &pos) {
	if (entType == Entity381Types::CarrierType)
		m_entities.push_back(new Entity381Carrier(this->m_engine, "cvn68.mesh", m_entities.size(), pos));
	else if (entType == Entity381Types::DestroyerType)
		m_entities.push_back(new Entity381Destroyer(this->m_engine, "ddg51.mesh", m_entities.size(), pos));
	else if (entType == Entity381Types::SpeedBoatType)
		m_entities.push_back(new Entity381SpeedBoat(this->m_engine, "cigarette.mesh", m_entities.size(), pos));
	else if (entType == Entity381Types::FrigateType)
		m_entities.push_back(new Entity381Frigate(this->m_engine, "sleek.mesh", m_entities.size(), pos));
	else if (entType == Entity381Types::AlienType)
		m_entities.push_back(new Entity381Alien(this->m_engine, "alienship.mesh", m_entities.size(), pos));
	else if (entType == Entity381Types::BansheeType)
		m_entities.push_back(new Entity381Banshee(this->m_engine, "banshee.mesh", m_entities.size(), pos));
	else if (entType == Entity381Types::PlaneType)
		m_entities.push_back(new Entity381Plane(this->m_engine, "RZR-002.mesh", m_entities.size(), pos));
	else
		return nullptr; //unknown type

	return m_entities.back();
}

unsigned int EntityMgr::GetEntityCount(void) const {
	return m_entities.size();
}

void EntityMgr::SelectNextEntity(bool make_sound) {
	SelectEntity(m_entity_selected_index + 1, make_sound);
}

void EntityMgr::SelectEntity(unsigned int index, bool make_sound) {
	ClearEntityGroup();
	m_entity_selected_index = index % m_entities.size();
	m_entity_selected = m_entities[m_entity_selected_index];
	if (make_sound)
		m_entity_selected->MakeSelectionSound();
}

bool EntityMgr::AddEntityToGroup(unsigned int index, bool make_sound) {
	index %= m_entities.size();
	if (int(index) != m_entity_selected_index) {
		if (m_entities_selected.insert(index).second && make_sound) {
			m_entities[index]->MakeSelectionSound();
		}
	}
	return false;
}

void EntityMgr::AddNextUnselectedToGroup(bool make_sound) {
	if (m_entity_selected_index != -1) {
		int i = m_entity_selected_index + 1;
		while (i != m_entity_selected_index) {
			i = i % m_entities.size();
			if (AddEntityToGroup(i++, make_sound))
				break;
		}
	}
}

void EntityMgr::ClearEntityGroup(void) {
	m_entities_selected.clear();
}

Entity381* EntityMgr::GetMainSelected(void) {
	return m_entity_selected;
}

const Entity381* EntityMgr::GetMainSelected(void) const {
	return m_entity_selected;
}

Entity381* EntityMgr::GetEntity(unsigned int index) {
	return m_entities[index];
}

const Entity381* EntityMgr::GetEntity(unsigned int index) const {
	return m_entities[index];
}

bool EntityMgr::IsEntitySelected(const Entity381* entity) const {
	if (entity == m_entity_selected)
		return true;

	for (unsigned int i : m_entities_selected) {
		if (m_entities[i] == entity)
			return true;
	}

	return false;
}

int EntityMgr::GetEntityIndex(const std::string &name) const {
	for (unsigned int i = 0; i < m_entities.size(); ++i) {
		if (name == m_entities[i]->GetName())
			return int(i);
	}
	return -1;
}

void EntityMgr::ChangeSelectedDesiredSpeed(float change, bool accumulate) {
	if (m_entity_selected) {
		if (accumulate)
			m_entity_selected->m_desired_speed += change;
		else
			m_entity_selected->m_desired_speed = change;
	}
	for (unsigned int i : m_entities_selected) {
		if (accumulate)
			m_entities[i]->m_desired_speed += change;
		else
			m_entities[i]->m_desired_speed = change;
	}
}

void EntityMgr::ChangeSelectedDesiredHeading(float change, bool accumulate) {
	if (m_entity_selected) {
		if (accumulate)
			m_entity_selected->m_desired_heading += change;
		else
			m_entity_selected->m_desired_heading = change;
	}
	for (unsigned int i : m_entities_selected) {
		if (accumulate)
			m_entities[i]->m_desired_heading += change;
		else
			m_entities[i]->m_desired_heading = change;
	}
}

void EntityMgr::ChangeSelectedDesiredPitch(float change, bool accumulate) {
	if (m_entity_selected) {
		if (accumulate)
			m_entity_selected->m_desired_pitch += change;
		else
			m_entity_selected->m_desired_pitch = change;
	}
	for (unsigned int i : m_entities_selected) {
		if (accumulate)
			m_entities[i]->m_desired_pitch += change;
		else
			m_entities[i]->m_desired_pitch = change;
	}
}

void EntityMgr::ChangeSelectedDesiredRoll(float change, bool accumulate) {
	if (m_entity_selected) {
		if (accumulate)
			m_entity_selected->m_desired_roll += change;
		else
			m_entity_selected->m_desired_roll = change;
	}
	for (unsigned int i : m_entities_selected) {
		if (accumulate)
			m_entities[i]->m_desired_roll += change;
		else
			m_entities[i]->m_desired_roll = change;
	}
}

void EntityMgr::StopSelectedEntities(void) {
	m_entity_selected->m_velocity = Ogre::Vector3::ZERO;
	m_entity_selected->m_desired_speed = 0;
	m_entity_selected->m_desired_heading = m_entity_selected->m_heading;
	m_entity_selected->RemoveAllCommands();
	for (unsigned int i : m_entities_selected) {
		m_entities[i]->m_velocity = Ogre::Vector3::ZERO;
		m_entities[i]->m_desired_speed = 0;
		m_entities[i]->m_desired_heading = m_entities[i]->m_heading;
		m_entities[i]->RemoveAllCommands();
	}
}

void EntityMgr::AddMoveToCommandToSelectedEntities(const Ogre::Vector3 &target, bool remove_past_commands) {
	m_entity_selected->AddCommand(new CommandMoveTo(m_entity_selected, target), remove_past_commands);
	for (unsigned int i : m_entities_selected)
		m_entities[i]->AddCommand(new CommandMoveTo(m_entities[i], target), remove_past_commands);
}

void EntityMgr::AddInterceptCommandToSelectedEntities(const Entity381* target, bool remove_past_commands) {
	if (m_entity_selected != target)
		m_entity_selected->AddCommand(new CommandIntercept(m_entity_selected, target), remove_past_commands);
	for (unsigned int i : m_entities_selected)
		if (m_entities[i] != target)
			m_entities[i]->AddCommand(new CommandIntercept(m_entities[i], target), remove_past_commands);
}

void EntityMgr::AddFollowCommandToSelectedEntities(const Entity381* target, bool remove_past_commands) {
	if (m_entity_selected != target)
		m_entity_selected->AddCommand(new CommandFollow(m_entity_selected, target), remove_past_commands);
	for (unsigned int i : m_entities_selected)
		if (m_entities[i] != target)
			m_entities[i]->AddCommand(new CommandFollow(m_entities[i], target), remove_past_commands);
}