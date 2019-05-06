#ifndef UTILS_H_
#define UTILS_H_

#include <string>

#include <OGRE/OgreMath.h>

float FixAngle(float angle);

void FixAngle(Ogre::Degree &a);

float Clamp(float min, float max, float val);

float ProgressValueTowards(float value, float desired, float max_change);

float ProgressAngleTowardsDesired(float angle, float desired, float max_change); //both angle and desired must be fixed

float Rand01(void);

#endif
