#pragma once

#include <maya/MGlobal.h>
#include <maya/MVector.h>

#include <random>

class DLCommon
{
public:

	//ENUMS
	enum DLRandomMode { kNormal, kTranslate, kRotate, kScale };

	//METHODS
	void dlCopyFloat3(const float3& copyFrom, float3& copyTo);

	static float dlGenerateRandomValues(int inSeed, float maxDifference, DLRandomMode mode);

	static MVector dlGenerateRandomValues(int inSeed, float3 maxDifference, DLRandomMode mode);


private:
	static int getModeSeed(DLRandomMode mode);
};
