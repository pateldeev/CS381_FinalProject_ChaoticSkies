#include <OGRE/OgreRenderWindow.h>

#include "Engine.h"

#include "GfxMgr.h"
#include "InputMgr.h"
#include "GameMgr.h"
#include "EntityMgr.h"
#include "UIMgr.h"

#include "Entity381.h"

InputMgr::InputMgr(Engine *engine) :
	Mgr(engine), m_ois_mgr(nullptr), m_keyboard(nullptr), m_mouse(nullptr), m_ray_query(nullptr) {
	m_multiselect_start[0] = m_multiselect_start[1] = m_multiselect_end[0] = m_multiselect_end[1] = 0.f;
}

InputMgr::~InputMgr(void) {
	Stop();
}

void InputMgr::Init(void) {
	OIS::ParamList pl;
	size_t window_handle = 0;
	std::ostringstream window_handle_str;

	m_engine->GetGfxMgr()->GetOgreRenderWindow()->getCustomAttribute("WINDOW", &window_handle);
	window_handle_str << window_handle;
	pl.insert(std::make_pair(std::string("WINDOW"), window_handle_str.str()));

#if defined OIS_WIN32_PLATFORM
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
#elif defined OIS_LINUX_PLATFORM
	pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
	pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
	pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("true")));
	pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif
	m_ois_mgr = OIS::InputManager::createInputSystem(pl);

	m_keyboard = static_cast<OIS::Keyboard*>(m_ois_mgr->createInputObject(OIS::OISKeyboard, true));
	m_mouse = static_cast<OIS::Mouse*>(m_ois_mgr->createInputObject(OIS::OISMouse, true));

	int left, top;
	unsigned int width, height, depth;
	m_engine->GetGfxMgr()->GetOgreRenderWindow()->getMetrics(width, height, depth, left, top);
	const OIS::MouseState &ms = m_mouse->getMouseState();
	ms.width = width;
	ms.height = height;

	m_mouse->setEventCallback(this);
	m_keyboard->setEventCallback(this);
}

void InputMgr::LoadLevel(void) {
	m_ray_query = m_engine->GetGfxMgr()->GetOgreSceneManager()->createRayQuery(Ogre::Ray());
}

void InputMgr::Tick(float dt) {
	m_mouse->capture();
	m_keyboard->capture();
	if (m_keyboard->isKeyDown(OIS::KC_ESCAPE))
		m_engine->StopRunning();
}

void InputMgr::Stop(void) {
	if (m_ois_mgr) {
		m_ois_mgr->destroyInputObject(m_mouse);
		m_ois_mgr->destroyInputObject(m_keyboard);

		OIS::InputManager::destroyInputSystem(m_ois_mgr);
		m_ois_mgr = nullptr;
		m_mouse = nullptr;
		m_keyboard = nullptr;
	}
}

bool InputMgr::keyPressed(const OIS::KeyEvent &key) {
	m_engine->GetGameMgr()->InjectKeyPress(key.key);
	return true;
}

bool InputMgr::keyReleased(const OIS::KeyEvent &key) {
	m_engine->GetGameMgr()->InjectKeyRelease(key.key);
	return true;
}

bool InputMgr::mouseMoved(const OIS::MouseEvent &me) {
	if (m_engine->GetUIMgr()->InjectMouseMovement(me))
		return true;

	if (m_mouse->getMouseState().buttonDown(OIS::MouseButtonID::MB_Left)) {
		m_multiselect_end[0] = float(me.state.X.abs);
		m_multiselect_end[1] = float(me.state.Y.abs);
	}
	return true;
}

bool InputMgr::mousePressed(const OIS::MouseEvent &me, OIS::MouseButtonID id) {
	if (m_engine->GetUIMgr()->InjectMousePress(me, id))
		return true;

	if (id == OIS::MouseButtonID::MB_Left) {
		m_multiselect_start[0] = float(me.state.X.abs);
		m_multiselect_start[1] = float(me.state.Y.abs);
	}

	if (id == OIS::MouseButtonID::MB_Left || id == OIS::MouseButtonID::MB_Right || id == OIS::MouseButtonID::MB_Middle) {

		int entity_clicked = GetEntityClickedOn(float(me.state.X.abs), float(me.state.Y.abs));

		//check in 3x3 square region to ensure nothing was close
		if (entity_clicked == -1)
			entity_clicked = GetEntityClickedOn(float(me.state.X.abs) + 1, float(me.state.Y.abs));
		if (entity_clicked == -1)
			entity_clicked = GetEntityClickedOn(float(me.state.X.abs) - 1, float(me.state.Y.abs));
		if (entity_clicked == -1)
			entity_clicked = GetEntityClickedOn(float(me.state.X.abs), float(me.state.Y.abs) + 1);
		if (entity_clicked == -1)
			entity_clicked = GetEntityClickedOn(float(me.state.X.abs), float(me.state.Y.abs) - 1);
		if (entity_clicked == -1)
			entity_clicked = GetEntityClickedOn(float(me.state.X.abs) + 1, float(me.state.Y.abs) + 1);
		if (entity_clicked == -1)
			entity_clicked = GetEntityClickedOn(float(me.state.X.abs) + 1, float(me.state.Y.abs) - 1);
		if (entity_clicked == -1)
			entity_clicked = GetEntityClickedOn(float(me.state.X.abs) - 1, float(me.state.Y.abs) + 1);
		if (entity_clicked == -1)
			entity_clicked = GetEntityClickedOn(float(me.state.X.abs) - 1, float(me.state.Y.abs) - 1);

		if (entity_clicked >= 0) {
			if (id == OIS::MouseButtonID::MB_Left && !m_keyboard->isKeyDown(OIS::KC_RSHIFT)) { //select clicked object
				std::cout << std::endl << "SUCCESS: Now selecting entity nearest click!" << std::endl;
				if (m_keyboard->isKeyDown(OIS::KC_LSHIFT))
					m_engine->GetEntityMgr()->AddEntityToGroup(entity_clicked);
				else
					m_engine->GetEntityMgr()->SelectEntity(entity_clicked);
			} else if (id == OIS::MouseButtonID::MB_Right) { //intercept/follow clicked object
				if (m_engine->GetEntityMgr()->IsEntitySelected(m_engine->GetEntityMgr()->GetEntity(entity_clicked))) {
					std::cout << std::endl << "FAILURE: An entity cannot intercept/follow itself!" << std::endl;
				} else {
					if (m_keyboard->isKeyDown(OIS::KC_LCONTROL)) { //follow
						std::cout << std::endl << "SUCCESS: Now having selected entities follow clicked entity!" << std::endl;
						m_engine->GetEntityMgr()->AddFollowCommandToSelectedEntities(m_engine->GetEntityMgr()->GetEntity(entity_clicked), !m_keyboard->isKeyDown(OIS::KC_LSHIFT));
					} else { //intercept
						std::cout << std::endl << "SUCCESS: Now having selected entities intercept clicked entity!" << std::endl;
						m_engine->GetEntityMgr()->AddInterceptCommandToSelectedEntities(m_engine->GetEntityMgr()->GetEntity(entity_clicked), !m_keyboard->isKeyDown(OIS::KC_LSHIFT));
					}
				}
			}
		} else if (id == OIS::MouseButtonID::MB_Middle || id == OIS::MouseButtonID::MB_Right) { //set desired location
			Ogre::Ray ray = m_engine->GetGfxMgr()->CreateOgreRay(float(me.state.X.abs), float(me.state.Y.abs));
			std::pair<bool, Ogre::Real> ground_intersection = ray.intersects(Ogre::Plane(Ogre::Vector3(0.f, 1.f, 0.f), 0.f));
			Ogre::Vector3 click_loc = ray.getPoint(ground_intersection.second);
			click_loc.y = 0.f;
			std::cout << std::endl << "SUCCESS: Having selected entities go to location: " << click_loc << std::endl;
			m_engine->GetEntityMgr()->AddMoveToCommandToSelectedEntities(click_loc, !m_keyboard->isKeyDown(OIS::KC_LSHIFT));
		}
	}

	return true;
}

bool InputMgr::mouseReleased(const OIS::MouseEvent &me, OIS::MouseButtonID id) {
	if (m_engine->GetUIMgr()->InjectMouseRelease(me, id))
		return true;

	if (id == OIS::MouseButtonID::MB_Left && m_keyboard->isKeyDown(OIS::KC_RSHIFT)) { //perform group selection

		bool primary_selected = false;
		m_engine->GetEntityMgr()->ClearEntityGroup();

		for (float x = std::min(m_multiselect_start[0], m_multiselect_end[0]); x < std::max(m_multiselect_start[0], m_multiselect_end[0]); ++x) {
			for (float y = std::min(m_multiselect_start[1], m_multiselect_end[1]); y < std::max(m_multiselect_start[1], m_multiselect_end[1]); ++y) {

				int entity_clicked = GetEntityClickedOn(x, y);

				if (entity_clicked >= 0) {
					if (!primary_selected) {
						m_engine->GetEntityMgr()->SelectEntity(entity_clicked);
						primary_selected = true;
					} else {
						m_engine->GetEntityMgr()->AddEntityToGroup(entity_clicked, false);
					}
				}
			}
		}
	}
	return true;
}

bool InputMgr::IsKeyPressed(const OIS::KeyCode &key) const {
	return m_keyboard->isKeyDown(key);
}

const OIS::MouseState& InputMgr::GetMouseState(void) const {
	return m_mouse->getMouseState();
}

OIS::Keyboard* InputMgr::GetOISKeyboard(void) {
	return m_keyboard;
}

OIS::Mouse* InputMgr::GetOISMouse(void) {
	return m_mouse;
}

int InputMgr::GetEntityClickedOn(float x, float y) const {
	//execute ray cast to find entity being clicked on
	m_ray_query->setRay(m_engine->GetGfxMgr()->CreateOgreRay(x, y));
	m_ray_query->setSortByDistance(true);
	Ogre::RaySceneQueryResult &result = m_ray_query->execute();

	for (Ogre::RaySceneQueryResult::iterator result_iter = result.begin(); result_iter != result.end(); ++result_iter) {
		if (result_iter->movable) {
			int entity_hit = m_engine->GetEntityMgr()->GetEntityIndex(result_iter->movable->getParentSceneNode()->getName());

			if (entity_hit >= 0)
				return entity_hit;
		}
	}
	return -1; //nothing found
}
