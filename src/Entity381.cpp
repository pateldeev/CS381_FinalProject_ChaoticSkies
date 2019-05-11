#include "Entity381.h"

#include "GfxMgr.h"
#include "SoundMgr.h"
#include "Utils.h"

unsigned int Entity381::id_generator = 0;

Entity381::~Entity381(void) {
	for (Aspect *a : m_aspects)
		delete a;
	m_scene_node->getParentSceneNode()->detachObject(m_ogre_entity);
	m_engine->GetGfxMgr()->GetOgreSceneManager()->destroySceneNode(m_scene_node->getName());
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
	m_engine->GetSoundMgr()->PauseAllAudio();
	m_engine->GetSoundMgr()->PlayAudio(m_selection_sound);
	m_engine->GetSoundMgr()->ResumeAllAudio();
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

void Entity381::AddSmoke(void) {
	if (!m_smoke_node) {
		Ogre::ParticleSystem* smoke = m_engine->GetGfxMgr()->GetOgreSceneManager()->createParticleSystem("smoke_" + m_name, "Crash/Smoke");
		m_smoke_node = m_scene_node->createChildSceneNode("smoke_" + m_name + "_node");
		m_smoke_node->attachObject(smoke);
	}
}
void Entity381::ShowBoundingBox(void) const {
	m_scene_node->showBoundingBox(true);
}

void Entity381::HideBoundingBox(void) const {
	m_scene_node->showBoundingBox(false);
}

unsigned int Entity381::GetId(void) const {
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

void Entity381::Yaw(const Ogre::Degree &rate, bool override_max) {
	m_yaw_rate = (override_max) ? rate : Ogre::Math::Clamp(rate, -m_yaw_rate_max, m_yaw_rate_max);
}

void Entity381::YawLeft(void) {
	m_yaw_rate = m_yaw_rate_max;
}

void Entity381::YawRight(void) {
	m_yaw_rate = -m_yaw_rate_max;
}

void Entity381::YawStop(void) {
	m_yaw_rate = 0;
}

Ogre::Degree Entity381::GetYaw(void) const {
	return GetRotationLocal().getYaw();
}

Ogre::Degree Entity381::GetYawRate(void) const {
	return m_yaw_rate;
}

void Entity381::Pitch(const Ogre::Degree &rate, bool override_max) {
	m_pitch_rate = (override_max) ? rate : Ogre::Math::Clamp(rate, -m_pitch_rate_max, m_pitch_rate_max);
}

void Entity381::PitchUp(void) {
	m_pitch_rate = -m_pitch_rate_max;
}

void Entity381::PitchDown(void) {
	m_pitch_rate = m_pitch_rate_max;
}

void Entity381::PitchStop(void) {
	m_pitch_rate = 0;
}

Ogre::Degree Entity381::GetPitch(void) const {
	return GetRotationLocal().getPitch();
}

Ogre::Degree Entity381::GetPitchRate(void) const {
	return m_pitch_rate;
}

void Entity381::Roll(const Ogre::Degree &rate, bool override_max) {
	m_roll_rate = (override_max) ? rate : Ogre::Math::Clamp(rate, -m_roll_rate_max, m_roll_rate_max);
}

void Entity381::RollLeft(void) {
	m_roll_rate = -m_roll_rate_max;
}

void Entity381::RollRight(void) {
	m_roll_rate = m_roll_rate_max;
}

void Entity381::RollStop(void) {
	m_roll_rate = 0;
}

Ogre::Degree Entity381::GetRoll(void) const {
	return GetRotationLocal().getRoll();
}

Ogre::Degree Entity381::GetRollRate(void) const {
	return m_roll_rate;
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

Ogre::Vector3 Entity381::GetDirection(void) const {
#if 0
	Ogre::Vector3 v = Ogre::Quaternion(GetYaw(), Ogre::Vector3::UNIT_Y) * Ogre::Vector3::UNIT_X;
	v.normalise();

	Ogre::Matrix3 m;
	Ogre::Radian y, p, r;
	m_rotation.ToRotationMatrix(m);
	m.ToEulerAnglesYXZ(y, p, r);

	v.y = -Ogre::Math::Sin(p);

	return v.normalisedCopy();
#else
	return (m_scene_node->_getDerivedOrientation() * Ogre::Vector3::UNIT_Z).normalisedCopy();
#endif
}

Ogre::Quaternion Entity381::GetRotationLocal(void) const {
	return m_rotation;
}

Ogre::Quaternion Entity381::GetRotationWorld(void) const {
	return m_rotation_mesh * m_rotation;
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

float Entity381::GetAcceleration(void) const {
	return m_acceleration;
}

float Entity381::GetTurnRate(void) const {
	return m_turn_rate;
}

void Entity381::Kill(void) {
	m_is_alive = false;
}

bool Entity381::IsAlive(void) const {
	return m_is_alive;
}

bool Entity381::Is3D(void) const {
	return m_is3D;
}

Entity381::Entity381(Engine *engine, const std::string &mesh, bool apply_3Dphysics, const std::string &selection_sound_file, const Ogre::Vector3 &pos, const Ogre::Quaternion &mesh_rotatation) :
	m_speed_min(0), m_speed_max(0), m_acceleration(0), m_turn_rate(0), m_yaw_rate_max(0), m_pitch_rate_max(0), m_roll_rate_max(0), m_rotation(Ogre::Quaternion::IDENTITY), m_rotation_mesh(mesh_rotatation), m_engine(engine), m_id(id_generator++),
	m_name(mesh + std::to_string(m_id)), m_mesh_file(mesh), m_selection_sound(selection_sound_file), m_speed(0), m_speed_desired(0), m_heading(0), m_heading_desired(0), m_yaw_rate(0), m_pitch_rate(0), m_roll_rate(0), m_ogre_entity(nullptr),
	m_scene_node(nullptr), m_smoke_node(nullptr), m_position(pos), m_velocity(0.f), m_AI_aspect(nullptr), m_is_alive(true), m_is3D(apply_3Dphysics) {

	m_ogre_entity = m_engine->GetGfxMgr()->GetOgreSceneManager()->createEntity(m_mesh_file);
	m_scene_node = m_engine->GetGfxMgr()->GetOgreSceneManager()->getRootSceneNode()->createChildSceneNode(m_name, m_position);
	m_scene_node->attachObject(m_ogre_entity);

	m_AI_aspect = new AspectUnitAI(this);
	m_aspects.push_back(static_cast<Aspect*>(m_AI_aspect));

	if (m_is3D)
		m_aspects.push_back(new AspectPhysics3D(this));
	else
		m_aspects.push_back(new AspectPhysics2D(this));

	m_aspects.push_back(new AspectRenderable(this));
}

void Entity381::UpdateRotation(float dt) {
	m_rotation = Ogre::Quaternion(GetYawRate() * dt, Ogre::Vector3::UNIT_Y) * m_rotation * Ogre::Quaternion(GetRollRate() * dt, Ogre::Vector3::UNIT_Z) * Ogre::Quaternion(GetPitchRate() * dt, Ogre::Vector3::UNIT_X);
}

