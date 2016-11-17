#pragma once

#include <maya/MIntArray.h>
#include <maya/MPointArray.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MMatrixArray.h>

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
	MMatrixArray normalAlignmentMatricies;
	float normalOffset;
	MVector translateOffset;
	MVector rotationOffset;
	float uniformScaleOffset;
	MVector scaleOffset;
	MFloatArray normalRandom;
	MFloatVectorArray translateRandom;
	MFloatVectorArray rotationRandom;
	MFloatArray uniformScaleRandom;
	MFloatVectorArray scaleRandom;


};


