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

void Entity381::LoadAudio(void) {
	m_engine->GetSoundMgr()->LoadAudio(m_selection_sound, &m_audio_id, false);
}

void Entity381::FixMeshOrientation(float &yaw, float &pitch, float &roll) {
}

void Entity381::ShowBoundingBox(void) const {
	m_scene_node->showBoundingBox(true);
}

void Entity381::TurnOffBoundingBox(void) const {
	m_scene_node->showBoundingBox(false);
}

Ogre::Vector3 Entity381::GetPosition(void) const {
	return m_position;
}

Ogre::Vector3 Entity381::GetVelocity(void) const {
	return m_velocity;
}

std::string Entity381::GetName(void) const {
	return m_name;
}

Ogre::SceneNode* Entity381::GetOgreSceneNode(void) {
	return m_scene_node;
}

const Ogre::SceneNode* Entity381::GetOgreSceneNode(void) const {
	return m_scene_node;
}

float Entity381::GetSpeed(void) const {
	return m_speed;
}

float Entity381::GetSpeedDesired(void) const {
	return m_desired_speed;
}

void Entity381::SetSpeedDesired(float speed, bool accumulate) {
	if (accumulate)
		m_desired_speed += speed;
	else
		m_desired_speed = speed;
	m_desired_speed = Clamp(m_speed_min, m_speed_max, m_desired_speed);
}

float Entity381::GetHeading(void) const {
	return m_heading;
}

float Entity381::GetHeadingDesired(void) const {
	return m_desired_heading;
}

void Entity381::SetHeadingDesired(float heading, bool accumulate) {
	if (accumulate)
		m_desired_heading += heading;
	else
		m_desired_heading = heading;
}

float Entity381::GetPitch(void) const {
	return m_pitch;
}

float Entity381::GetPitchDesired(void) const {
	return m_desired_pitch;
}

void Entity381::SetPitchDesired(float pitch, bool accumulate) {
	if (accumulate)
		m_desired_pitch += pitch;
	else
		m_desired_pitch = pitch;
}

float Entity381::GetRoll(void) const {
	return m_roll;
}

float Entity381::GetRollDesired(void) const {
	return m_desired_roll;
}

void Entity381::SetRollDesired(float roll, bool accumulate) {
	if (accumulate)
		m_desired_roll += roll;
	else
		m_desired_roll = roll;
}

float Entity381::GetSpeedMax(void) const {
	return m_speed_max;
}

const AspectUnitAI* Entity381::GetAIAspect(void) const {
	return m_aspect_ai;
}

