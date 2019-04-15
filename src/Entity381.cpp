#include "Entity381.h"

#include "GfxMgr.h"
#include "InputMgr.h"
#include "SoundMgr.h"
#include "GameMgr.h"
#include "EntityMgr.h"

#include "Utils.h"

Entity381::Entity381(Engine *engine, const std::string &meshfilename, int identity, const Ogre::Vector3 &pos, const Ogre::Quaternion &rotate) :
	m_engine(engine), m_id(identity), m_name(meshfilename + IntToString(m_id)), m_mesh_file(meshfilename), m_scene_node(nullptr),
	m_ogre_entity(nullptr), m_acceleration(0.f), m_turn_rate(0.f), m_speed_min(0.f), m_speed_max(0.f), m_selection_sound(""), m_audio_id(0),
	m_position(pos), m_velocity(0.f), m_desired_heading(0.f), m_desired_speed(0.f), m_desired_pitch(0.f), m_desired_roll(0.f),
	m_heading(m_desired_heading), m_speed(m_desired_speed), m_pitch(m_desired_pitch), m_roll(m_desired_roll), m_aspect_ai(nullptr) {

	m_scene_node = engine->GetGfxMgr()->GetOgreSceneManager()->getRootSceneNode()->createChildSceneNode(m_name, pos, rotate);
	m_ogre_entity = engine->GetGfxMgr()->GetOgreSceneManager()->createEntity(m_mesh_file);
	m_scene_node->attachObject(m_ogre_entity);

	m_aspects.push_back(new AspectRenderable(this));
	m_aspect_ai = new AspectUnitAI(this);
	m_aspects.push_back((Aspect*) m_aspect_ai);
}

Entity381::~Entity381(void) {
	for (Aspect *a : m_aspects)
		delete a;
}

void Entity381::Tick(float dt) {
	m_scene_node->showBoundingBox(false);
	for (Aspect *a : m_aspects)
		a->Tick(dt);
	m_aspect_ai->DrawBoundingBox();
}

std::string Entity381::GetName(void) const {
	return m_name;
}

void Entity381::MakeSelectionSound(void) {
	m_engine->GetSoundMgr()->StopAllAudio();
	m_engine->GetSoundMgr()->PlayAudio(m_audio_id, true);
}

void Entity381::RemoveAllCommands(void) {
	m_aspect_ai->RemoveAllCommands();
}

void Entity381::AddCommand(Command *c, bool remove_past) {
	if (remove_past)
		m_aspect_ai->SetCommand(c);
	else
		m_aspect_ai->AddCommand(c);
}

void Entity381::FixMeshOrientation(float &yaw, float &pitch, float &roll) {

}
