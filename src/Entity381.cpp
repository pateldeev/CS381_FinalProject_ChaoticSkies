#include "Entity381.h"

#include "GfxMgr.h"
#include "InputMgr.h"
#include "SoundMgr.h"
#include "GameMgr.h"
#include "EntityMgr.h"

#include "Utils.h"

Entity381::~Entity381(void) {
	for (Aspect *a : m_aspects)
		delete a;
}

void Entity381::Tick(float dt) {
	m_scene_node->showBoundingBox(false);
	for (Aspect *a : m_aspects)
		a->Tick(dt);
	m_AI_aspect->DrawBoundingBox();
}

void Entity381::LoadAudio(void) {
	m_engine->GetSoundMgr()->LoadAudio(m_selection_sound, m_selection_sound);
}

void Entity381::MakeSelectionSound(void) {
	m_engine->GetSoundMgr()->StopAllAudio();
	m_engine->GetSoundMgr()->PlayAudio(m_selection_sound);
}

void Entity381::RemoveAllCommands(void) {
	m_AI_aspect->RemoveAllCommands();
}

void Entity381::AddCommand(Command *c, bool remove_past) {
	if (remove_past)
		m_AI_aspect->SetCommand(c);
	else
		m_AI_aspect->AddCommand(c);
}

void Entity381::GetMeshOrientationsFixed(float &yaw, float &pitch, float &roll) const {
}

void Entity381::ShowBoundingBox(void) const {
	m_scene_node->showBoundingBox(true);
}

void Entity381::HideBoundingBox(void) const {
	m_scene_node->showBoundingBox(false);
}

int Entity381::GetId(void) const {
	return m_id;
}

std::string Entity381::GetName(void) const {
	return m_name;
}

float Entity381::GetSpeed(void) const {
	return m_speed;
}

float Entity381::GetSpeedDesired(void) const {
	return m_speed_desired;
}

void Entity381::SetSpeedDesired(float speed, bool accumulate) {
	if (accumulate)
		m_speed_desired += speed;
	else
		m_speed_desired = speed;
	m_speed_desired = Clamp(m_speed_min, m_speed_max, m_speed_desired);
}

float Entity381::GetHeading(void) const {
	return m_heading;
}

float Entity381::GetHeadingDesired(void) const {
	return m_heading_desired;
}

void Entity381::SetHeadingDesired(float heading, bool accumulate) {
	if (accumulate)
		m_heading_desired += heading;
	else
		m_heading_desired = heading;
}

float Entity381::GetPitch(void) const {
	return m_pitch;
}

void Entity381::PitchUp(void) {
	m_pitch_rate_current = 0.5 * m_pitch_rate_max;
}

void Entity381::PitchDown(void) {
	m_pitch_rate_current = -0.5 * m_pitch_rate_max;
}

void Entity381::PitchStop(void) {
	m_pitch_rate_current = 0;
}

float Entity381::GetRoll(void) const {
	return m_roll;
}

void Entity381::RollLeft(void) {
	m_roll_rate_current = 0.5 * m_roll_rate_max;
}

void Entity381::RollRight(void) {
	m_roll_rate_current = -0.5 * m_roll_rate_max;
}

void Entity381::RollStop(void) {
	m_roll_rate_current = 0;
}

Ogre::SceneNode* Entity381::GetOgreSceneNode(void) {
	return m_scene_node;
}

const Ogre::SceneNode* Entity381::GetOgreSceneNode(void) const {
	return m_scene_node;
}

Ogre::Vector3 Entity381::GetPosition(void) const {
	return m_position;
}

Ogre::Vector3 Entity381::GetVelocity(void) const {
	return m_velocity;
}

const AspectUnitAI* Entity381::GetAIAspect(void) const {
	return m_AI_aspect;
}

float Entity381::GetSpeedMax(void) const {
	return m_speed_max;
}

float Entity381::GetSpeedMin(void) const {
	return m_speed_min;
}

Entity381::Entity381(Engine *engine, const std::string &mesh, int id, bool apply_3Dphysics, const std::string &selection_sound_file,
	const Ogre::Vector3 &pos, const Ogre::Quaternion &rotate) :
	m_speed_min(0), m_speed_max(0), m_acceleration(0), m_turn_rate(0), m_pitch_rate_max(0), m_roll_rate_max(0), m_engine(engine), m_id(id),
	m_name(mesh + IntToString(m_id)), m_mesh_file(mesh), m_selection_sound(selection_sound_file), m_speed(0), m_speed_desired(0), m_heading(0),
	m_heading_desired(0), m_pitch(0), m_pitch_rate_current(0), m_roll(0), m_roll_rate_current(0), m_ogre_entity(nullptr), m_scene_node(nullptr),
	m_position(pos), m_velocity(0.f), m_AI_aspect(nullptr) {

	m_ogre_entity = engine->GetGfxMgr()->GetOgreSceneManager()->createEntity(m_mesh_file);
	m_scene_node = engine->GetGfxMgr()->GetOgreSceneManager()->getRootSceneNode()->createChildSceneNode(m_name, pos, rotate);
	m_scene_node->attachObject(m_ogre_entity);

	m_aspects.push_back(new AspectRenderable(this));
	m_AI_aspect = new AspectUnitAI(this);
	m_aspects.push_back(static_cast<Aspect*>(m_AI_aspect));

	if (apply_3Dphysics)
		m_aspects.push_back(new AspectPhysics3D(this));
	else
		m_aspects.push_back(new AspectPhysics2D(this));
}

