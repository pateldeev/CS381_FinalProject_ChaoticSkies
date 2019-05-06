#include "Utils.h"

float FixAngle(float angle) {
	if (angle < 0)
		return FixAngle(angle + 360);
	else if (angle >= 360)
		return FixAngle(angle - 360);
	return angle;
}

void FixAngle(Ogre::Degree &a) {
	float f = a.valueDegrees();
	if (f > 180)
		a -= Ogre::Degree(360);
	else if (f < -180)
		a += Ogre::Degree(360);
	else
		return;
	FixAngle(a);
}

float Clamp(float min, float max, float val) {
	if (val < min) {
		return min;
	} else if (val > max) {
		return max;
	}
	return val;
}

float ProgressValueTowards(float value, float desired, float max_change) {
	if (std::abs(value - desired) <= max_change)
		return value;
	else
		return (desired > value) ? value + max_change : value - max_change;
}

float ProgressAngleTowardsDesired(float angle, float desired, float max_change) { //both angle and desired must be fixed
	if (std::abs(angle - desired) <= max_change)
		return angle;
	else if (desired > angle)
		return FixAngle((desired - angle < 180) ? (angle + max_change) : (angle - max_change));
	else
		return FixAngle((desired - angle < -180) ? (angle + max_change) : (angle - max_change));
}

float Rand01(void) {
	return float(double(std::rand()) / RAND_MAX);
}
