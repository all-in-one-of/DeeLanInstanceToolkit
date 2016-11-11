#pragma once

#include <maya/MIntArray.h>
#include <maya/MPointArray.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatVectorArray.h>

#include <vector>

struct DLMeshData
{
	bool initialized;
	unsigned int numPoints;
	unsigned int numPolys;
	MPointArray pointArray;
	MIntArray polyCounts;
	MIntArray polyConnects;
	MFloatVectorArray normals;
	MFloatArray uArray;
	MFloatArray vArray;
	MIntArray uvIDs;
	MIntArray uvCounts;


	void appendArrayData(const DLMeshData& dataToAppend);

};



struct DLTransformData
{
	bool initialized;
	MPointArray referencePoints;
	std::vector<float3> normals;
	std::vector<float3> normalRotations;
	float normalOffset;
	float3 translateOffset;
	float3 rotationOffset;
	float3 scaleOffset;
	float normalRandom;
	float3 translateRandom;
	float3 rotationRandom;
	float3 scaleRandom;


};


