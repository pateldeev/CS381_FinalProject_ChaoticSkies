#ifndef UTILS_H_
#define UTILS_H_

#include <string>

float FixAngle(float angle);

float Clamp(float min, float max, float val);

float ProgressValueTowards(float value, float desired, float max_change);

std::string IntToString(int x);

float ProgressAngleTowardsDesired(float angle, float desired, float max_change); //both angle and desired must be fixed

#endif
