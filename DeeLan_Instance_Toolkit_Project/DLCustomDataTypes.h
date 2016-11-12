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
	MFloatVectorArray referenceNormals;
	MFloatVectorArray normalRotations;
	float normalOffset;
	float3 translateOffset;
	float3 rotationOffset;
	float3 scaleOffset;
	std::vector<float> normalRandom;
	std::vector<float3> translateRandom;
	std::vector<float3> rotationRandom;
	std::vector<float3> scaleRandom;


};


