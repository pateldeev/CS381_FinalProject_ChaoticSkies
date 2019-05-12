#include <OGRE/OgreMeshManager.h>

#include "Engine.h"

#include "GfxMgr.h"
#include "InputMgr.h"
#include "SoundMgr.h"
#include "GameMgr.h"
#include "EntityMgr.h"
#include "UIMgr.h"

#include "Entity381.h"

float GameMgr::fire_cooldown = 0.4;

GameMgr::GameMgr(Engine *engine) :
	Mgr(engine), m_camera_node(nullptr), m_camera_following(nullptr), m_plane(nullptr), m_enemies_following(0), m_camera_manual_control(OIS::KC_UNASSIGNED), m_desired_control(OIS::KC_UNASSIGNED), m_fire_cooldown(0), m_levels_won(0) {
}

GameMgr::~GameMgr(void) {
	Stop();
}

void GameMgr::Init(void) {
	m_bullet_sound = "assets/sounds/shooting.ogg";
	m_explosion_sound = "assets/sounds/Explosion.ogg";
}

void GameMgr::LoadLevel(void) {
	MakeCamera();
	MakeGround();
	MakeSky();
	MakeLighting();
	MakeEntities();

	m_engine->GetSoundMgr()->LoadAudio(m_explosion_sound, m_explosion_sound);
	m_engine->GetSoundMgr()->LoadAudio(m_bullet_sound, m_bullet_sound);
}

void GameMgr::Tick(float dt) {
	if (!m_plane->IsAlive() || m_plane->GetPosition().y <= 5)
		LoseLevel();

	UpdateCamera(dt);
	UpdateSelectedDesiredAtributes(dt);
	HandleBulletsAndFiring(dt);
	RemoveDeadEntities();

	//std::cout << "Pitch " << std::fmod(m_plane->GetPitch(), 360) << " |||Roll " << std::fmod(m_plane->GetRoll(), 360) << std::endl;
	//Ogre::Quaternion q = m_plane->GetRotationLocal();
	//Ogre::Vector3 x, y, z;
	//q.ToAxes(x, y, z);
	//Ogre::Radian yaw = q.getYaw(), pitch = q.getPitch(), roll = q.getRoll();
	//std::cout << q << "|" << x << "," << y << "," << z << "|||" << yaw.valueDegrees() << "," << pitch.valueDegrees() << "," << roll.valueDegrees() << std::endl;
	//std::cout << m_plane->GetYaw() << "|" << m_plane->GetPitch() << "|" << m_plane->GetRoll() << std::endl;
	//std::cout << x << "," << y << "," << z << std::endl;

	std::cout << "Enemies Following: " << m_enemies_following << std::endl;
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

	if (key == OIS::KC_R && m_engine->GetInputMgr()->IsKeyPressed(OIS::KC_LCONTROL)) {
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

	if (key == OIS::KC_A || key == OIS::KC_D || key == OIS::KC_W || key == OIS::KC_S || key == OIS::KC_F || key == OIS::KC_R || key == OIS::KC_Q || key == OIS::KC_E || key == OIS::KC_Z || key == OIS::KC_X) {
		m_camera_manual_control = key;
		m_camera_following = nullptr;
		return;
	}

	//handle tab selection
	if (key == OIS::KC_TAB) {
		if (m_engine->GetInputMgr()->IsKeyPressed(OIS::KC_LSHIFT))
			m_engine->GetEntityMgr()->AddNextUnselectedToGroup();
		else
			m_engine->GetEntityMgr()->SelectNextEntity();
		return;
	}

	//handle 3D rotation of plane
	if (m_engine->GetInputMgr()->IsKeyPressed(OIS::KC_LCONTROL)) {
		if (key == OIS::KC_UP && !m_engine->GetInputMgr()->IsKeyPressed(OIS::KC_DOWN))
			m_plane->AddCommand(new CommandPitch(m_plane, false), true);
		else if (key == OIS::KC_DOWN && !m_engine->GetInputMgr()->IsKeyPressed(OIS::KC_UP))
			m_plane->AddCommand(new CommandPitch(m_plane, true), true);
		else if (key == OIS::KC_LEFT && !m_engine->GetInputMgr()->IsKeyPressed(OIS::KC_RIGHT))
			m_plane->AddCommand(new CommandRoll(m_plane, false), true);
		else if (key == OIS::KC_RIGHT && !m_engine->GetInputMgr()->IsKeyPressed(OIS::KC_LEFT))
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

	if (key == OIS::KC_N) { //toggle 3rd person camera mode
		CrashPlane();
		return;
	}
}

void GameMgr::InjectKeyRelease(const OIS::KeyCode &key) {
	if (key == m_desired_control)
		m_desired_control = OIS::KC_UNASSIGNED;

	if (key == m_camera_manual_control)
		m_camera_manual_control = OIS::KC_UNASSIGNED;

	if (key == OIS::KC_UP && !m_engine->GetInputMgr()->IsKeyPressed(OIS::KC_DOWN))
		m_engine->GetEntityMgr()->StopSelectedPitch();
	else if (key == OIS::KC_DOWN && !m_engine->GetInputMgr()->IsKeyPressed(OIS::KC_UP))
		m_engine->GetEntityMgr()->StopSelectedPitch();
	else if (key == OIS::KC_LEFT && !m_engine->GetInputMgr()->IsKeyPressed(OIS::KC_RIGHT))
		m_engine->GetEntityMgr()->StopSelectedRoll();
	else if (key == OIS::KC_RIGHT && !m_engine->GetInputMgr()->IsKeyPressed(OIS::KC_LEFT))
		m_engine->GetEntityMgr()->StopSelectedRoll();
}

void GameMgr::CrashPlane(void) {
	m_plane->AddCommand(new CommandDie(m_plane), true);
	m_camera_following = nullptr;
}

void GameMgr::WinLevel(void) {
	std::cout << "WON LEVEL" << std::endl;

	++m_levels_won;
	if (m_levels_won > 2) {
		m_engine->GetUIMgr()->ShowBackdrop();
		// needs a pauser
		m_engine->StopRunning();
	}

	ResetLevel();
}

void GameMgr::LoseLevel(void) {
	std::cout << "LEVEL LOST" << std::endl;
	ResetLevel();
}

void GameMgr::ResetLevel(void) {
	for (Bullet* b : m_bullets)
		delete b;
	m_bullets.clear();

	m_enemies.clear();
	m_engine->GetEntityMgr()->DeleteAllEntities();

	SetCameraStateToDefault();
	MakeEntities();

	m_engine->GetUIMgr()->PlayFlightSound();
	m_engine->GetUIMgr()->SetHealthBarPercentage(100);
}

int GameMgr::GetLevelsWon(void) const {
	return m_levels_won;
}

void GameMgr::SetLevelsWon(int levels) {
	m_levels_won = levels;
}

unsigned int GameMgr::GetNumEnemies(void) const {
	return m_enemies.size();
}

unsigned int GameMgr::GetNumEnemiesFollowing(void) const {
	return m_enemies_following;
}

void GameMgr::AddEnemyFollowing(void) {
	++m_enemies_following;
}

void GameMgr::RemoveEnemyFollowing(void) {
	--m_enemies_following;
}

void GameMgr::MakeCamera(void) {
	//m_engine->GetGfxMgr()->GetOgreCamera()->lookAt(Ogre::Vector3(0, 0, 0));
	m_camera_node = m_engine->GetGfxMgr()->GetOgreSceneManager()->getRootSceneNode()->createChildSceneNode();
	m_camera_node->attachObject(m_engine->GetGfxMgr()->GetOgreCamera());
	SetCameraStateToDefault();
}

void GameMgr::MakeGround(void) {
	Ogre::MeshManager::getSingleton().createPlane("ocean", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::Plane(Ogre::Vector3::UNIT_Y, 0), 15000, 15000, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);

	Ogre::Entity* ground_entity = m_engine->GetGfxMgr()->GetOgreSceneManager()->createEntity("ocean");
	Ogre::SceneNode* ground_node = m_engine->GetGfxMgr()->GetOgreSceneManager()->getRootSceneNode()->createChildSceneNode("ocean");
	ground_node->attachObject(ground_entity);
	ground_entity->setCastShadows(false);
	ground_entity->setMaterialName("OceanHLSL_GLSL"); //("Ocean2_HLSL_GLSL");//("Ocean2_Cg");//("NavyCg");
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
	MakeBoats();
	MakePlaneMain((m_levels_won == 0) ? 65 : 75);
	MakeEnemies(m_levels_won + 1);

	//Ogre::ParticleSystem* fireworks = m_engine->GetGfxMgr()->GetOgreSceneManager()->createParticleSystem("smoke", "Examples/Fireworks");
	//Ogre::SceneNode* fireworks_node = m_camera_following->GetOgreSceneNode()->createChildSceneNode("fireworks");
	//fireworks_node->attachObject(fireworks);
}

void GameMgr::MakeBoats(void) {
	float change_x_per_object;

	//3 carriers
	change_x_per_object = 1200.f;
	for (int i = -1; i <= 1; ++i)
		m_engine->GetEntityMgr()->CreateEntityOfTypeAtPosition(Entity381Types::CarrierType, Ogre::Vector3(i * change_x_per_object, 0, -1000.f));

	//4 frigates
	change_x_per_object = 800.f;
	for (int i = -1; i <= 2; ++i)
		m_engine->GetEntityMgr()->CreateEntityOfTypeAtPosition(Entity381Types::FrigateType, Ogre::Vector3(i * change_x_per_object, 0, -500.f));

	//5 destroyers
	change_x_per_object = 1000.f;
	for (int i = -2; i <= 2; ++i)
		m_engine->GetEntityMgr()->CreateEntityOfTypeAtPosition(Entity381Types::DestroyerType, Ogre::Vector3(i * change_x_per_object, 0, 50.f));

	//6 speedboats
	change_x_per_object = 700.f;
	for (int i = -3; i <= 2; ++i)
		m_engine->GetEntityMgr()->CreateEntityOfTypeAtPosition(Entity381Types::SpeedBoatType, Ogre::Vector3(i * change_x_per_object, 0, 300.f));

	//4 alienships
	change_x_per_object = 750.f;
	for (int i = -1; i <= 2; ++i)
		m_engine->GetEntityMgr()->CreateEntityOfTypeAtPosition(Entity381Types::AlienType, Ogre::Vector3(i * change_x_per_object, 0, 800.f));
}

void GameMgr::MakePlaneMain(int speed) {
	//add main plane
	m_plane = m_engine->GetEntityMgr()->CreateEntityOfTypeAtPosition(Entity381Types::PlaneType, Ogre::Vector3(0, 85, 500));
	//m_plane = m_engine->GetEntityMgr()->CreateEntityOfTypeAtPosition(Entity381Types::BansheeType, Ogre::Vector3(0, 75, 0));

	m_engine->GetEntityMgr()->SelectEntity(m_engine->GetEntityMgr()->GetEntityCount() - 1);	//sets selection

	m_plane->SetSpeedDesired(speed);
	//m_camera_following = m_plane;
}

void GameMgr::MakeEnemies(int level) {
	if (level == 1) {
		m_enemies.push_back(m_engine->GetEntityMgr()->CreateEntityOfTypeAtPosition(Entity381Types::BansheeType, Ogre::Vector3(-1200, 85, -600)));
		m_enemies.push_back(m_engine->GetEntityMgr()->CreateEntityOfTypeAtPosition(Entity381Types::BansheeType, Ogre::Vector3(-1200, 85, 600)));
		m_enemies.push_back(m_engine->GetEntityMgr()->CreateEntityOfTypeAtPosition(Entity381Types::BansheeType, Ogre::Vector3(0, 85, 0)));
		m_enemies.push_back(m_engine->GetEntityMgr()->CreateEntityOfTypeAtPosition(Entity381Types::BansheeType, Ogre::Vector3(1200, 85, -600)));
		m_enemies.push_back(m_engine->GetEntityMgr()->CreateEntityOfTypeAtPosition(Entity381Types::BansheeType, Ogre::Vector3(1200, 85, 600)));
	} else if (level == 2) {

	}

	Ogre::Vector3 extent(600, 75, 600);
	for (Entity381* e : m_enemies)
		e->AddCommand(new CommandPatrol(e, m_plane, e->GetPosition(), extent));

	//m_camera_following = m_enemies.front();
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
			move_direction.x -= (m_engine->GetInputMgr()->IsKeyPressed(OIS::KC_LSHIFT)) ? MOVE_FAST : MOVE_SLOW;
		else if (m_camera_manual_control == OIS::KC_D)
			move_direction.x += (m_engine->GetInputMgr()->IsKeyPressed(OIS::KC_LSHIFT)) ? MOVE_FAST : MOVE_SLOW;
		else if (m_camera_manual_control == OIS::KC_W)
			move_direction.z -= (m_engine->GetInputMgr()->IsKeyPressed(OIS::KC_LSHIFT)) ? MOVE_FAST : MOVE_SLOW;
		else if (m_camera_manual_control == OIS::KC_S)
			move_direction.z += (m_engine->GetInputMgr()->IsKeyPressed(OIS::KC_LSHIFT)) ? MOVE_FAST : MOVE_SLOW;
		else if (m_camera_manual_control == OIS::KC_R)
			move_direction.y += (m_engine->GetInputMgr()->IsKeyPressed(OIS::KC_LSHIFT)) ? MOVE_FAST : MOVE_SLOW;
		else if (m_camera_manual_control == OIS::KC_F)
			move_direction.y -= (m_engine->GetInputMgr()->IsKeyPressed(OIS::KC_LSHIFT)) ? MOVE_FAST : MOVE_SLOW;
		else if (m_camera_manual_control == OIS::KC_Q)
			m_camera_node->yaw(dt * Ogre::Degree((m_engine->GetInputMgr()->IsKeyPressed(OIS::KC_LSHIFT)) ? ROTATE_FAST : ROTATE_SLOW));
		else if (m_camera_manual_control == OIS::KC_E)
			m_camera_node->yaw(dt * -1 * Ogre::Degree((m_engine->GetInputMgr()->IsKeyPressed(OIS::KC_LSHIFT)) ? ROTATE_FAST : ROTATE_SLOW));
		else if (m_camera_manual_control == OIS::KC_Z)
			m_camera_node->pitch(dt * Ogre::Degree((m_engine->GetInputMgr()->IsKeyPressed(OIS::KC_LSHIFT)) ? ROTATE_FAST : ROTATE_SLOW));
		else if (m_camera_manual_control == OIS::KC_X)
			m_camera_node->pitch(dt * -1 * Ogre::Degree((m_engine->GetInputMgr()->IsKeyPressed(OIS::KC_LSHIFT)) ? ROTATE_FAST : ROTATE_SLOW));
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
	m_camera_node->setPosition(m_camera_following->GetOgreSceneNode()->_getDerivedPosition());
	m_camera_node->resetOrientation();

	if (m_camera_following->Is3D()) {
		Ogre::Matrix3 m;
		Ogre::Radian y, p, r;
		m_camera_following->GetRotationLocal().ToRotationMatrix(m);
		m_camera_node->yaw(Ogre::Degree(-90));
		m.ToEulerAnglesYXZ(y, p, r);
		m_camera_node->yaw(y);
		m_camera_node->pitch(-p);
		m_camera_node->roll(-r);
	} else {
		m_camera_node->yaw(Ogre::Degree(m_camera_following->GetHeading()));
	}

	m_camera_node->translate(Ogre::Vector3(0, 4, 32), Ogre::Node::TransformSpace::TS_LOCAL);

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
					m_engine->GetSoundMgr()->PlayAudio(m_explosion_sound);
					e->AddCommand(new CommandDie(e), true);
					m_engine->GetSoundMgr()->PauseAllAudio();
					m_engine->GetSoundMgr()->PlayAudio(m_explosion_sound);
					m_engine->GetSoundMgr()->ResumeAllAudio();
					(*b)->Deactivate();
				}
			}
		}
	}

	//handle firing
	m_fire_cooldown -= dt;
	if (m_engine->GetInputMgr()->IsKeyPressed(OIS::KC_SPACE) && m_fire_cooldown < 0) {
		Ogre::Vector3 d = m_plane->GetDirection();
		m_bullets.push_front(new Bullet(m_engine, m_plane->GetPosition() + 15 * d, d));
		m_engine->GetSoundMgr()->PlayAudio(m_bullet_sound);
		m_fire_cooldown = fire_cooldown;
		m_engine->GetSoundMgr()->PauseAllAudio();
		m_engine->GetSoundMgr()->PlayAudio(m_bullet_sound);
		m_engine->GetSoundMgr()->ResumeAllAudio();
	}
}

void GameMgr::RemoveDeadEntities(void) {
	for (std::list<Entity381*>::iterator i = m_enemies.begin(); i != m_enemies.end(); ++i) {
		if (!(*i)->IsAlive()) {
			m_engine->GetEntityMgr()->DeleteEntity(*i);
			i = m_enemies.erase(i);
		}
	}

	if (m_enemies.empty())
		WinLevel();
}

void GameMgr::SetCameraStateToDefault(void) {
	m_camera_node->resetOrientation();

#if 0
	m_camera_node->setPosition(0, 50, 75);
#else
	m_camera_node->setPosition(0, 3000, 0);
	m_camera_node->pitch(Ogre::Degree(-90));
#endif

	m_camera_following = nullptr;
}

