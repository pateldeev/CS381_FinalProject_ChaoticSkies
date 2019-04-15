#include "Entity381SubClasses/Entity381Plane.h"

#include "Utils.h"

Entity381Plane::Entity381Plane(Engine *engine, const std::string &meshfilename, int identity, const Ogre::Vector3 &pos) :
	Entity381(engine, meshfilename, identity, pos, Ogre::Quaternion(-0.7071068, 0.f, 0.7071068, 0.f)) {
	this->m_speed_min = 0;
	this->m_speed_max = 250.0f; //meters per second...
	this->m_acceleration = 30.0f; // slow
	this->m_turn_rate = 10.f;//45.0f; //2 degrees per second

	m_selection_sound = "assets/sounds/Selection_Banshee.ogg";

	m_aspects.push_back(new AspectPhysics3D(this));

}

Entity381Plane::~Entity381Plane(void) {
}

void Entity381Plane::FixMeshOrientation(float &yaw, float &pitch, float &roll) {
	yaw += 90;
	yaw = FixAngle(yaw);
	pitch = -pitch;
	roll = -roll;
}

