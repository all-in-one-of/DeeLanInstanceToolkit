#pragma once

#include <maya/MGlobal.h>

void dlCopyFloat3(const float3& copyFrom, float3& copyTo);

void dlGenerateRandomValues(unsigned int seed, unsigned int seedOffset, float maxDifference, float& values);

void dlGenerateRandomValues(unsigned int seed, unsigned int seedOffset, float3 maxDifference, float3& values);