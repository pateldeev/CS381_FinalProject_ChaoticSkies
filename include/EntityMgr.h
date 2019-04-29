#ifndef __ENTITYMGR_h_
#define __ENTITYMGR_h_

#include <vector>
#include <unordered_set>

#include "Mgr.h"
#include "Entity381.h"

class EntityMgr: public Mgr {
public:
	EntityMgr(Engine* engine);
	virtual ~EntityMgr(void);

public:
	virtual void Init(void) override final;
	virtual void LoadLevel(void) override final;
	virtual void Tick(float dt) override final;
	virtual void Stop(void) override final;

public:
	Entity381* CreateEntityOfTypeAtPosition(Entity381Types type, const Ogre::Vector3 &pos);
	void DeleteAllEntities(void);
	bool DeleteEntity(Entity381* entity);

	unsigned int GetEntityCount(void) const;

	void SelectNextEntity(bool make_sound = true);
	void SelectEntity(unsigned int index, bool make_sound = true);
	bool AddEntityToGroup(unsigned int index, bool make_sound = true);
	void AddNextUnselectedToGroup(bool make_sound = true);
	void ClearEntityGroup(void);

	Entity381* GetMainSelected(void);
	const Entity381* GetMainSelected(void) const;
	Entity381* GetEntity(unsigned int index);
	const Entity381* GetEntity(unsigned int index) const;
	bool IsEntitySelected(const Entity381* entity) const;

	int GetEntityIndex(const std::string &name) const;

	void ChangeSelectedDesiredSpeed(float change, bool accumulate = true);
	void ChangeSelectedDesiredHeading(float change, bool accumulate = true);

	void PitchSelectedUp(void);
	void PitchSelectedDown(void);
	void StopSelectedPitch(void);

	void RollSelectedLeft(void);
	void RollSelectedRight(void);
	void StopSelectedRoll(void);

	void StopSelectedEntities(void);

	void AddMoveToCommandToSelectedEntities(const Ogre::Vector3 &target, bool remove_past_commands = false);
	void AddInterceptCommandToSelectedEntities(const Entity381* target, bool remove_past_commands = false);
	void AddFollowCommandToSelectedEntities(const Entity381* target, bool remove_past_commands = false);

private:
	std::vector<Entity381*> m_entities;
	Entity381* m_entity_selected;
	int m_entity_selected_index;
	std::unordered_set<unsigned int> m_entities_selected; //hold entities selected in group - not main one
};

//#define EDIT_BOUNDINGBOX_COLOR_EXPERIMENAL //experimental function that does unsafe static casting to access protected member of Ogre::SceneNode to change bounding box color
#ifdef EDIT_BOUNDINGBOX_COLOR_EXPERIMENAL
#include <OGRE/OgreWireBoundingBox.h>
class BoudingBoxColorChangerExperimenatal: public Ogre::SceneNode {
public:
	void MakeBoundingBoxGreen(void) {
		CreateNewMatertials();
		if (!mWireBoundingBox)
			mWireBoundingBox = new Ogre::WireBoundingBox();
		mWireBoundingBox->setMaterial("BaseGreenLighting");
	}
	void MakeBoundingBoxBlue(void) {
		CreateNewMatertials();
		if (!mWireBoundingBox)
			mWireBoundingBox = new Ogre::WireBoundingBox();
		mWireBoundingBox->setMaterial("BaseBlueLighting");
	}
	void MakeBoundingBoxRed(void) {
		CreateNewMatertials();
		if (!mWireBoundingBox)
			mWireBoundingBox = new Ogre::WireBoundingBox();
		mWireBoundingBox->setMaterial("BaseRedLighting");
	}
	void MakeBoundingBoxOrange(void) {
		CreateNewMatertials();
		if (!mWireBoundingBox)
			mWireBoundingBox = new Ogre::WireBoundingBox();
		mWireBoundingBox->setMaterial("BaseOrangeLighting");
	}
	void MakeBoundingBoxBlack(void) {
		CreateNewMatertials();
		if (!mWireBoundingBox)
			mWireBoundingBox = new Ogre::WireBoundingBox();
		mWireBoundingBox->setMaterial("BaseBlackLighting");
	}

	static void CreateNewMatertials(void) {
		static bool done = false;
		if (!done) {
			Ogre::MaterialPtr(Ogre::MaterialManager::getSingleton().getByName("BaseWhiteNoLighting"))->setLightingEnabled(true);

			//green
			Ogre::MaterialPtr mat_green = ((Ogre::MaterialPtr) Ogre::MaterialManager::getSingleton().getByName("BaseWhiteNoLighting"))->clone(
				"BaseGreenLighting");
			mat_green->setAmbient(0.f, 1.f, 0.f);
			mat_green->setDiffuse(0.f, 1.f, 0.f, 0.f);

			//blue
			Ogre::MaterialPtr mat_blue = ((Ogre::MaterialPtr) Ogre::MaterialManager::getSingleton().getByName("BaseWhiteNoLighting"))->clone(
				"BaseBlueLighting");
			mat_blue->setAmbient(0.f, 0.f, 1.f);
			mat_blue->setDiffuse(0.f, 0.f, 1.f, 0.f);

			//red
			Ogre::MaterialPtr mat_red = ((Ogre::MaterialPtr) Ogre::MaterialManager::getSingleton().getByName("BaseWhiteNoLighting"))->clone(
				"BaseRedLighting");
			mat_red->setAmbient(1.f, 0.f, 0.f);
			mat_red->setDiffuse(1.f, 0.f, 0.f, 0.f);

			//orange
			Ogre::MaterialPtr mat_orange = ((Ogre::MaterialPtr) Ogre::MaterialManager::getSingleton().getByName("BaseWhiteNoLighting"))->clone(
				"BaseOrangeLighting");
			mat_orange->setAmbient(1.f, 0.65, 0.f);
			mat_orange->setDiffuse(1.f, 0.65, 0.f, 0.f);

			//black
			Ogre::MaterialPtr mat_black = ((Ogre::MaterialPtr) Ogre::MaterialManager::getSingleton().getByName("BaseWhiteNoLighting"))->clone(
				"BaseBlackLighting");
			mat_black->setAmbient(0.f, 0.f, 0.f);
			mat_black->setDiffuse(0.f, 0.f, 0.f, 0.f);

			done = true;
		}
	}
};
#endif

#endif
