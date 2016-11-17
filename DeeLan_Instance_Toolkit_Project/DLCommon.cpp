#include "DLCommon.h"

int DLCommon::getModeSeed(DLRandomMode mode)
{
	int seed;

	if (mode == kNormal)
	{
		seed = 1234;
	}
	else if (mode == kTranslate)
	{
		seed = 2143;
	}
	else if (mode == kRotate)
	{
		seed = 3241;
	}
	else if (mode == kScale)
	{
		seed = 4312;
	}

	return seed;
}

void DLCommon::dlCopyFloat3(const float3 & copyFrom, float3 & copyTo)
{
	copyTo[0] = copyFrom[0];
	copyTo[1] = copyFrom[1];
	copyTo[2] = copyFrom[2];
}

float DLCommon::dlGenerateRandomValues(int inSeed, float maxDifference, DLRandomMode mode)
{
	int modeSeed = getModeSeed(mode);
	int seed = inSeed + modeSeed;
	std::mt19937 randGen(seed);
	
	float deviation = maxDifference / 2;
	std::uniform_real_distribution<float> offset(-deviation, deviation);
	float outFloat = offset(randGen);
	return outFloat;
}

MVector DLCommon::dlGenerateRandomValues(int inSeed, float3 maxDifference, DLRandomMode mode)
{
	int modeSeed = getModeSeed(mode);
	MVector outVec;

	for (int i = 0; i < 3; ++i)
	{
		int indexSeed = i * 13;
		int seed = inSeed + modeSeed + indexSeed;
		std::mt19937 randGen(seed);

		float deviation = maxDifference[i] / 2;
		std::uniform_real_distribution<float> offset(-deviation, deviation);

		outVec[i] = offset(randGen);
	}
	return outVec;
}

