#include <OGRE/OgreMeshManager.h>

#include "GfxMgr.h"
#include "InputMgr.h"
#include "SoundMgr.h"
#include "GameMgr.h"
#include "EntityMgr.h"

#include "Entity381.h"

#include "Utils.h"

GameMgr::GameMgr(Engine *engine) :
	Mgr(engine), m_camera_node(nullptr), m_camera_following(nullptr), m_camera_manual_control(OIS::KC_UNASSIGNED),
	m_desired_control(OIS::KC_UNASSIGNED) {
}

GameMgr::~GameMgr(void) {
	Stop();
}

void GameMgr::Init(void) {
}

void GameMgr::LoadLevel(void) {
	MakeCamera();
	MakeGround();
	MakeSky();
	MakeLighting();
	MakeEntities();
}

void GameMgr::Tick(float dt) {
	UpdateCamera(dt);
	UpdateSelectedDesiredAtributes(dt);
}

void GameMgr::Stop(void) {

}

void GameMgr::InjectKeyPress(const OIS::KeyCode& key) {
	if (key == OIS::KC_T) { //toggle 3rd person camera mode
		m_camera_following = (m_camera_following) ? nullptr : m_engine->GetEntityMgr()->GetMainSelected();
		return;
	}

	if (key == OIS::KC_R && m_engine->GetIngputMgr()->IsKeyPressed(OIS::KC_LCONTROL)) {
		SetCameraStateToDefault();
		return;
	}

	if (key == OIS::KC_UP || key == OIS::KC_DOWN || key == OIS::KC_LEFT || key == OIS::KC_RIGHT || key == OIS::KC_I || key == OIS::KC_K
		|| key == OIS::KC_J || key == OIS::KC_L) {
		m_desired_control = key;
		return;
	} else if (key == OIS::KC_SPACE) {
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
}

void GameMgr::InjectKeyRelease(const OIS::KeyCode &key) {
	if (key == m_desired_control)
		m_desired_control = OIS::KC_UNASSIGNED;

	if (key == m_camera_manual_control)
		m_camera_manual_control = OIS::KC_UNASSIGNED;
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

	//create 2 banshee
	change_x_per_object = 150.f;
	for (unsigned int i = 0; i < 2; ++i)
		m_engine->GetEntityMgr()->CreateEntityOfTypeAtPosition(Entity381Types::BansheeType, Ogre::Vector3(i * change_x_per_object, 0, -100.f));

	m_engine->GetEntityMgr()->CreateEntityOfTypeAtPosition(Entity381Types::PlaneType, Ogre::Vector3(18, 50, -50));

	m_engine->GetEntityMgr()->SelectEntity(m_engine->GetEntityMgr()->GetEntityCount() - 1); //sets selection
}

void GameMgr::UpdateSelectedDesiredAtributes(float dt) {
	static float next_update_time = 0.f;

	if (m_desired_control != OIS::KC_UNASSIGNED) {
		if (next_update_time <= 0.f) {
			const static float DELTA_DESIRED_SPEED = 10.0f;
			const static float DELTA_DESIRED_ANGLE = 10.0f;

			if (m_desired_control == OIS::KC_I)
				m_engine->GetEntityMgr()->ChangeSelectedDesiredSpeed(DELTA_DESIRED_SPEED);
			else if (m_desired_control == OIS::KC_K)
				m_engine->GetEntityMgr()->ChangeSelectedDesiredSpeed(-DELTA_DESIRED_SPEED);
			else if (m_desired_control == OIS::KC_J)
				m_engine->GetEntityMgr()->ChangeSelectedDesiredHeading(DELTA_DESIRED_ANGLE);
			else if (m_desired_control == OIS::KC_L)
				m_engine->GetEntityMgr()->ChangeSelectedDesiredHeading(-DELTA_DESIRED_ANGLE);
			else if (m_desired_control == OIS::KC_UP)
				m_engine->GetEntityMgr()->ChangeSelectedDesiredPitch(DELTA_DESIRED_ANGLE);
			else if (m_desired_control == OIS::KC_DOWN)
				m_engine->GetEntityMgr()->ChangeSelectedDesiredPitch(-DELTA_DESIRED_ANGLE);
			else if (m_desired_control == OIS::KC_LEFT)
				m_engine->GetEntityMgr()->ChangeSelectedDesiredRoll(DELTA_DESIRED_ANGLE);
			else if (m_desired_control == OIS::KC_RIGHT)
				m_engine->GetEntityMgr()->ChangeSelectedDesiredRoll(-DELTA_DESIRED_ANGLE);
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
	m_camera_node->setPosition(m_camera_following->m_scene_node->_getDerivedPosition());
	m_camera_node->translate(Ogre::Vector3(-2, 4, 0), Ogre::Node::TransformSpace::TS_LOCAL);
#endif

	m_camera_node->resetOrientation();
	m_camera_node->yaw(Ogre::Degree(FixAngle(m_camera_following->m_heading - 90)));
	m_camera_node->pitch(Ogre::Degree(m_camera_following->m_pitch));
	m_camera_node->roll(Ogre::Degree(m_camera_following->m_roll));

}

void GameMgr::SetCameraStateToDefault(void) {
	m_camera_node->resetToInitialState();
	m_camera_node->translate(0, 200, 500, Ogre::Node::TransformSpace::TS_LOCAL);
	m_camera_following = nullptr;
}

