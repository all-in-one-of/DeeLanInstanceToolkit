#pragma once

#include <maya/MGlobal.h>
#include <maya/MVector.h>

#include <random>

enum DLRandomMode { kNormal, kTranslate, kRotate, kScale };

int getModeSeed(DLRandomMode mode);

void dlCopyFloat3(const float3& copyFrom, float3& copyTo);

float dlGenerateRandomValues(int inSeed, float maxDifference, DLRandomMode mode);

MVector dlGenerateRandomValues(int inSeed, float3 maxDifference, DLRandomMode mode);