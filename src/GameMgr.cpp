#include <OGRE/OgreMeshManager.h>

#include "Engine.h"

#include "GfxMgr.h"
#include "InputMgr.h"
#include "SoundMgr.h"
#include "GameMgr.h"
#include "EntityMgr.h"
#include "UIMgr.h"

#include "Entity381.h"

#include "Utils.h"

GameMgr::GameMgr(Engine *engine) :
	Mgr(engine), m_camera_node(nullptr), m_camera_following(nullptr), m_plane(nullptr), m_camera_manual_control(OIS::KC_UNASSIGNED),
	m_desired_control(OIS::KC_UNASSIGNED), m_fire_cooldown(0) {
}

GameMgr::~GameMgr(void) {
	Stop();
}

void GameMgr::Init(void) {
	m_flight_sound = "assets/sounds/inflight.ogg";
	m_bullet_sound = "assets/sounds/shooting.ogg";
}

void GameMgr::LoadLevel(void) {
	MakeCamera();
	MakeGround();
	MakeSky();
	MakeLighting();
	MakeEntities();

	m_engine->GetSoundMgr()->LoadAudio(m_flight_sound, m_flight_sound);
	m_engine->GetSoundMgr()->LoadAudio(m_bullet_sound, m_bullet_sound);
	m_engine->GetSoundMgr()->PlayAudio(m_flight_sound);
}

void GameMgr::Tick(float dt) {
	UpdateCamera(dt);
	UpdateSelectedDesiredAtributes(dt);
	HandleBulletsAndFiring(dt);
//std::cout << m_plane->GetRoll() << std::endl;
}

void GameMgr::Stop(void) {
	for (Bullet* b : m_bullets)
		delete b;
	m_bullets.clear();
}

void GameMgr::InjectKeyPress(const OIS::KeyCode& key) {
	if (key == OIS::KC_T) { //toggle 3rd person camera mode
		m_camera_following = (m_camera_following) ? nullptr : m_engine->GetEntityMgr()->GetMainSelected();
		return;
	}

	if (key == OIS::KC_R && m_engine->GetIngputMgr()->IsKeyPressed(OIS::KC_LCONTROL)) {
		ResetLevel();
		return;
	}

	if (key == OIS::KC_I || key == OIS::KC_K || key == OIS::KC_J || key == OIS::KC_L) {
		m_desired_control = key;
		return;
	} else if (key == OIS::KC_B) {
		m_engine->GetEntityMgr()->StopSelectedEntities();
		return;
	}

	if (key == OIS::KC_A || key == OIS::KC_D || key == OIS::KC_W || key == OIS::KC_S || key == OIS::KC_F || key == OIS::KC_R || key == OIS::KC_Q
		|| key == OIS::KC_E || key == OIS::KC_Z || key == OIS::KC_X) {
		m_camera_manual_control = key;
		m_camera_following = nullptr;
		return;
	}

//handle tab selection
	if (key == OIS::KC_TAB) {
		if (m_engine->GetIngputMgr()->IsKeyPressed(OIS::KC_LSHIFT))
			m_engine->GetEntityMgr()->AddNextUnselectedToGroup();
		else
			m_engine->GetEntityMgr()->SelectNextEntity();
		return;
	}

//handle 3D rotation of plane
	if (m_engine->GetIngputMgr()->IsKeyPressed(OIS::KC_LCONTROL)) {
		if (key == OIS::KC_UP && !m_engine->GetIngputMgr()->IsKeyPressed(OIS::KC_DOWN))
			m_engine->GetEntityMgr()->StopSelectedPitch();
		else if (key == OIS::KC_DOWN && !m_engine->GetIngputMgr()->IsKeyPressed(OIS::KC_UP))
			m_engine->GetEntityMgr()->StopSelectedPitch();
		else if (key == OIS::KC_LEFT && !m_engine->GetIngputMgr()->IsKeyPressed(OIS::KC_RIGHT))
			m_plane->AddCommand(new CommandRoll(m_plane, false), true);
		else if (key == OIS::KC_RIGHT && !m_engine->GetIngputMgr()->IsKeyPressed(OIS::KC_LEFT))
			m_plane->AddCommand(new CommandRoll(m_plane, true), false);
	} else {
		if (key == OIS::KC_UP)
			m_engine->GetEntityMgr()->PitchSelectedUp();
		else if (key == OIS::KC_DOWN)
			m_engine->GetEntityMgr()->PitchSelectedDown();
		else if (key == OIS::KC_LEFT)
			m_engine->GetEntityMgr()->RollSelectedLeft();
		else if (key == OIS::KC_RIGHT)
			m_engine->GetEntityMgr()->RollSelectedRight();
	}
}

void GameMgr::InjectKeyRelease(const OIS::KeyCode &key) {
	if (key == m_desired_control)
		m_desired_control = OIS::KC_UNASSIGNED;

	if (key == m_camera_manual_control)
		m_camera_manual_control = OIS::KC_UNASSIGNED;

	if (key == OIS::KC_UP && !m_engine->GetIngputMgr()->IsKeyPressed(OIS::KC_DOWN))
		m_engine->GetEntityMgr()->StopSelectedPitch();
	else if (key == OIS::KC_DOWN && !m_engine->GetIngputMgr()->IsKeyPressed(OIS::KC_UP))
		m_engine->GetEntityMgr()->StopSelectedPitch();
	else if (key == OIS::KC_LEFT && !m_engine->GetIngputMgr()->IsKeyPressed(OIS::KC_RIGHT))
		m_engine->GetEntityMgr()->StopSelectedRoll();
	else if (key == OIS::KC_RIGHT && !m_engine->GetIngputMgr()->IsKeyPressed(OIS::KC_LEFT))
		m_engine->GetEntityMgr()->StopSelectedRoll();
}

void GameMgr::MakeCamera(void) {
	m_engine->GetGfxMgr()->GetOgreCamera()->lookAt(Ogre::Vector3(0, 0, 0));
	m_camera_node = m_engine->GetGfxMgr()->GetOgreSceneManager()->getRootSceneNode()->createChildSceneNode();
	m_camera_node->attachObject(m_engine->GetGfxMgr()->GetOgreCamera());
	SetCameraStateToDefault();
}

void GameMgr::MakeGround(void) {
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);

	Ogre::MeshManager::getSingleton().createPlane("ocean", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 15000, 15000, 20, 20, true,
		1, 5, 5, Ogre::Vector3::UNIT_Z);

	Ogre::Entity* groundEntity = m_engine->GetGfxMgr()->GetOgreSceneManager()->createEntity("ocean");
	Ogre::SceneNode* groundNode = m_engine->GetGfxMgr()->GetOgreSceneManager()->getRootSceneNode()->createChildSceneNode("ocean");
	groundNode->attachObject(groundEntity);
	groundEntity->setCastShadows(false);
	groundEntity->setMaterialName("OceanHLSL_GLSL");
//groundEntity->setMaterialName("Ocean2_HLSL_GLSL");
//groundEntity->setMaterialName("Ocean2_Cg");
//groundEntity->setMaterialName("NavyCg");
}

void GameMgr::MakeSky(void) {
	m_engine->GetGfxMgr()->GetOgreSceneManager()->setSkyBox(true, "Examples/MorningSkyBox");
}

void GameMgr::MakeLighting(void) {
	m_engine->GetGfxMgr()->GetOgreSceneManager()->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	Ogre::Light* light = m_engine->GetGfxMgr()->GetOgreSceneManager()->createLight("MainLight");
	light->setPosition(20.0, 80.0, 50.0);
}

void GameMgr::MakeEntities(void) {
	float change_x_per_object;

//create 3 carriers
	change_x_per_object = 700.f;
	for (unsigned int i = 0; i < 3; ++i)
		m_engine->GetEntityMgr()->CreateEntityOfTypeAtPosition(Entity381Types::CarrierType, Ogre::Vector3(i * change_x_per_object, 0, -1000.f));

//create 4 frigates
	change_x_per_object = 450.f;
	for (unsigned int i = 0; i < 4; ++i)
		m_engine->GetEntityMgr()->CreateEntityOfTypeAtPosition(Entity381Types::FrigateType, Ogre::Vector3(i * change_x_per_object, 0, -800.f));

//create 5 destroyers
	change_x_per_object = 250.f;
	for (unsigned int i = 0; i < 5; ++i)
		m_engine->GetEntityMgr()->CreateEntityOfTypeAtPosition(Entity381Types::DestroyerType, Ogre::Vector3(i * change_x_per_object, 0, -600.f));

//create 6 speedboats
	change_x_per_object = 200.f;
	for (unsigned int i = 0; i < 6; ++i)
		m_engine->GetEntityMgr()->CreateEntityOfTypeAtPosition(Entity381Types::SpeedBoatType, Ogre::Vector3(i * change_x_per_object, 0, -425.f));

//create 4 alienships
	change_x_per_object = 250.f;
	for (unsigned int i = 0; i < 4; ++i)
		m_engine->GetEntityMgr()->CreateEntityOfTypeAtPosition(Entity381Types::AlienType, Ogre::Vector3(i * change_x_per_object, 0, -250.f));

//create 5 banshees - enemies
	change_x_per_object = 450.f;
	for (unsigned int i = 0; i < 5; ++i)
		m_enemies.push_back(
			m_engine->GetEntityMgr()->CreateEntityOfTypeAtPosition(Entity381Types::BansheeType, Ogre::Vector3(i * change_x_per_object, 100, -100.f)));

	m_enemies.push_back(m_engine->GetEntityMgr()->CreateEntityOfTypeAtPosition(Entity381Types::BansheeType, Ogre::Vector3(2500, 50, -50)));

//add main plane
	m_plane = m_camera_following = m_engine->GetEntityMgr()->CreateEntityOfTypeAtPosition(Entity381Types::PlaneType, Ogre::Vector3(18, 50, -50));
	m_plane->SetSpeedDesired(60);

	m_engine->GetEntityMgr()->SelectEntity(m_engine->GetEntityMgr()->GetEntityCount() - 1);	//sets selection

//Ogre::ParticleSystem* fireworks = m_engine->GetGfxMgr()->GetOgreSceneManager()->createParticleSystem("smoke", "Examples/Fireworks");
//Ogre::SceneNode* fireworks_node = m_camera_following->GetOgreSceneNode()->createChildSceneNode("fireworks");
//fireworks_node->attachObject(fireworks);
}

void GameMgr::UpdateSelectedDesiredAtributes(float dt) {
	static float next_update_time = 0.f;

	if (m_desired_control != OIS::KC_UNASSIGNED) {
		if (next_update_time <= 0.f) {
			const static float DELTA_DESIRED_SPEED = 10.0f;
			const static float DELTA_DESIRED_HEADING = 10.0f;

			if (m_desired_control == OIS::KC_I)
				m_engine->GetEntityMgr()->ChangeSelectedDesiredSpeed(DELTA_DESIRED_SPEED);
			else if (m_desired_control == OIS::KC_K)
				m_engine->GetEntityMgr()->ChangeSelectedDesiredSpeed(-DELTA_DESIRED_SPEED);
			else if (m_desired_control == OIS::KC_J)
				m_engine->GetEntityMgr()->ChangeSelectedDesiredHeading(DELTA_DESIRED_HEADING);
			else if (m_desired_control == OIS::KC_L)
				m_engine->GetEntityMgr()->ChangeSelectedDesiredHeading(-DELTA_DESIRED_HEADING);
			next_update_time = 0.2;
			return;
		}
		next_update_time -= dt;
	} else {
		next_update_time = 0.f;
	}
}

void GameMgr::UpdateCamera(float dt) {
	if (m_camera_manual_control != OIS::KC_UNASSIGNED) {
		const static float MOVE_SLOW = 300.0f, MOVE_FAST = 600.0f;
		const static float ROTATE_SLOW = 15.0f, ROTATE_FAST = 30.0f;

		Ogre::Vector3 move_direction = Ogre::Vector3::ZERO;

		if (m_camera_manual_control == OIS::KC_A)
			move_direction.x -= (m_engine->GetIngputMgr()->IsKeyPressed(OIS::KC_LSHIFT)) ? MOVE_FAST : MOVE_SLOW;
		else if (m_camera_manual_control == OIS::KC_D)
			move_direction.x += (m_engine->GetIngputMgr()->IsKeyPressed(OIS::KC_LSHIFT)) ? MOVE_FAST : MOVE_SLOW;
		else if (m_camera_manual_control == OIS::KC_W)
			move_direction.z -= (m_engine->GetIngputMgr()->IsKeyPressed(OIS::KC_LSHIFT)) ? MOVE_FAST : MOVE_SLOW;
		else if (m_camera_manual_control == OIS::KC_S)
			move_direction.z += (m_engine->GetIngputMgr()->IsKeyPressed(OIS::KC_LSHIFT)) ? MOVE_FAST : MOVE_SLOW;
		else if (m_camera_manual_control == OIS::KC_R)
			move_direction.y += (m_engine->GetIngputMgr()->IsKeyPressed(OIS::KC_LSHIFT)) ? MOVE_FAST : MOVE_SLOW;
		else if (m_camera_manual_control == OIS::KC_F)
			move_direction.y -= (m_engine->GetIngputMgr()->IsKeyPressed(OIS::KC_LSHIFT)) ? MOVE_FAST : MOVE_SLOW;
		else if (m_camera_manual_control == OIS::KC_Q)
			m_camera_node->yaw(dt * Ogre::Degree((m_engine->GetIngputMgr()->IsKeyPressed(OIS::KC_LSHIFT)) ? ROTATE_FAST : ROTATE_SLOW));
		else if (m_camera_manual_control == OIS::KC_E)
			m_camera_node->yaw(dt * -1 * Ogre::Degree((m_engine->GetIngputMgr()->IsKeyPressed(OIS::KC_LSHIFT)) ? ROTATE_FAST : ROTATE_SLOW));
		else if (m_camera_manual_control == OIS::KC_Z)
			m_camera_node->pitch(dt * Ogre::Degree((m_engine->GetIngputMgr()->IsKeyPressed(OIS::KC_LSHIFT)) ? ROTATE_FAST : ROTATE_SLOW));
		else if (m_camera_manual_control == OIS::KC_X)
			m_camera_node->pitch(dt * -1 * Ogre::Degree((m_engine->GetIngputMgr()->IsKeyPressed(OIS::KC_LSHIFT)) ? ROTATE_FAST : ROTATE_SLOW));
		else
			return;

		m_camera_node->translate(move_direction * dt, Ogre::Node::TS_LOCAL);
		if (m_camera_node->_getDerivedPosition().y < 2)
			m_camera_node->translate(0, 2 - m_camera_node->_getDerivedPosition().y, 0, Ogre::Node::TS_LOCAL);
		return;
	}

	if (m_camera_following) //follow entity
		UpdateCameraToFollowEntity();
}
void GameMgr::UpdateCameraToFollowEntity(void) {

#if 0
	Ogre::Vector3 entity_size = m_camera_following->m_scene_node->_getWorldAABB().getSize();
	Ogre::Vector3 entity_loc = m_camera_following->m_scene_node->_getDerivedPosition();
	entity_loc.y += (1.6 * entity_size.y);
	m_camera_node->setPosition(entity_loc);
//m_camera_node->translate(0.f, 0.f, 1.5 * std::max(entity_size.x, entity_size.z), Ogre::Node::TransformSpace::TS_LOCAL);
#else
	m_camera_node->setPosition(m_camera_following->GetOgreSceneNode()->_getDerivedPosition());
	m_camera_node->translate(Ogre::Vector3(-2, 4, 0), Ogre::Node::TransformSpace::TS_LOCAL);
#endif

	m_camera_node->resetOrientation();
	m_camera_node->yaw(Ogre::Degree(FixAngle(m_camera_following->GetHeading() - 90)));
	m_camera_node->pitch(Ogre::Degree(m_camera_following->GetPitch()));
	m_camera_node->roll(Ogre::Degree(m_camera_following->GetRoll()));
}

void GameMgr::HandleBulletsAndFiring(float dt) {
	for (std::list<Bullet*>::iterator b = m_bullets.begin(); b != m_bullets.end(); ++b) {
		if (!((*b)->IsStillActive())) {
			delete *b;
			b = m_bullets.erase(b);
		} else {
			(*b)->Tick(dt);
			for (Entity381* e : m_enemies) {
				if ((*b)->HasCollidedWithInLastTick(e->GetPosition())) {
					e->AddSmoke();
					(*b)->Deactivate();
				}
			}
		}
	}

//handle firing
	m_fire_cooldown -= dt;
	if (m_engine->GetIngputMgr()->IsKeyPressed(OIS::KC_SPACE) && m_fire_cooldown < 0) {
		m_bullets.push_front(new Bullet(m_engine, m_plane->GetPosition() + 20 * m_plane->GetDirection(), m_plane->GetDirection()));
		//std::cout << m_plane->GetDirection() << std::endl;
		m_engine->GetSoundMgr()->PlayAudio(m_bullet_sound);
		m_fire_cooldown = 0.5;
		m_engine->GetSoundMgr()->PauseAllAudio();
		m_engine->GetSoundMgr()->PlayAudio(m_bullet_sound);
		m_engine->GetSoundMgr()->ResumeAllAudio();
	}
}

void GameMgr::SetCameraStateToDefault(void) {
	m_camera_node->resetToInitialState();
	m_camera_node->translate(0, 200, 500, Ogre::Node::TransformSpace::TS_LOCAL);
	m_camera_following = nullptr;
}

void GameMgr::ResetLevel(void) {
	for (Bullet* b : m_bullets)
		delete b;
	m_bullets.clear();

	m_enemies.clear();
	m_engine->GetEntityMgr()->DeleteAllEntities();

	MakeEntities();
}

