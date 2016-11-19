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

	static float dlGenerateRandomValues(int inSeed, float maxDifference, DLRandomMode mode);

	static MVector dlGenerateRandomValues(int inSeed, float3 maxDifference, DLRandomMode mode);


private:
	static int getModeSeed(DLRandomMode mode);
};
