#include <OGRE/OgreEntity.h>
#include <OGRE/OgreCamera.h>
#include <OGRE/OgreViewport.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreConfigFile.h>
#include <OGRE/OgreException.h>

#include <ois/OISMouse.h>

#include "Engine.h"

#include "GfxMgr.h"
#include "InputMgr.h"

GfxMgr::GfxMgr(Engine *engine) :
	Mgr(engine), m_root(nullptr), m_window(nullptr), m_scene_mgr(nullptr), m_camera(nullptr) {
	m_resources_file = Ogre::StringUtil::BLANK;
	m_plugins_file = Ogre::StringUtil::BLANK;
}

GfxMgr::~GfxMgr(void) {
	Stop();
	Ogre::WindowEventUtilities::removeWindowEventListener(m_window, this);
	windowClosed(m_window);
	delete m_root;
}

void GfxMgr::Init(void) {
	m_resources_file = "resources.cfg";
	m_plugins_file = "plugins.cfg";

	m_root = new Ogre::Root(m_plugins_file);

	Ogre::ConfigFile cf;
	cf.load(m_resources_file);

	Ogre::String name, locType;
	Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator();

	while (secIt.hasMoreElements()) {
		Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator it;

		for (it = settings->begin(); it != settings->end(); ++it) {
			locType = it->first;
			name = it->second;

			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, locType);
		}
	}

	if (!(m_root->restoreConfig() || m_root->showConfigDialog()))
		std::cerr << "Could not find Config File and could not show Config Dialog" << std::endl;

	m_window = m_root->initialise(true, "CS381_Final_Project");

	m_scene_mgr = m_root->createSceneManager(Ogre::ST_GENERIC);

	m_camera = m_scene_mgr->createCamera("camera");
	m_camera->setPosition(0, 0, 80);
	m_camera->lookAt(0, 0, -300);
	m_camera->setNearClipDistance(5);

	Ogre::Viewport* vp = m_window->addViewport(m_camera);
	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

	m_camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	Ogre::WindowEventUtilities::addWindowEventListener(m_window, this);
}

void GfxMgr::LoadLevel(void) {
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	windowResized(m_window);
}

void GfxMgr::Tick(float dt) {
	if (m_window->isClosed())
		m_engine->StopRunning();

	//call ogre root render one frame method
	m_root->renderOneFrame(dt);
	Ogre::WindowEventUtilities::messagePump();
}

void GfxMgr::Stop(void) {
	m_root->shutdown();
}

void GfxMgr::windowClosed(Ogre::RenderWindow *rw) {
	m_root->shutdown();
	m_engine->StopRunning();
}

void GfxMgr::windowResized(Ogre::RenderWindow *rw) {
	int left, top;
	unsigned int width, height, depth;

	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState& ms = m_engine->GetIngputMgr()->GetMouseState();
	ms.width = width;
	ms.height = height;
}

Ogre::SceneManager* GfxMgr::GetOgreSceneManager(void) {
	return m_scene_mgr;
}

const Ogre::SceneManager* GfxMgr::GetOgreSceneManager(void) const {
	return m_scene_mgr;
}

Ogre::RenderWindow* GfxMgr::GetOgreRenderWindow(void) {
	return m_window;
}

const Ogre::RenderWindow* GfxMgr::GetOgreRenderWindow(void) const {
	return m_window;
}

Ogre::Camera* GfxMgr::GetOgreCamera(void) {
	return m_camera;
}

const Ogre::Camera* GfxMgr::GetOgreCamera(void) const {
	return m_camera;
}

Ogre::Ray GfxMgr::CreateOgreRay(float x, float y) const {
	return m_camera->getCameraToViewportRay(x / float(m_window->getWidth()), y / float(m_window->getHeight()));
}

