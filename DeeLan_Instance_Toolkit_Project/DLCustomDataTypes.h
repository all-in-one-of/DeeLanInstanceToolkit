#pragma once

#include <maya/MIntArray.h>
#include <maya/MPointArray.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MMatrixArray.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>

#include <vector>

struct DLMeshData
{
	unsigned int numPoints;
	unsigned int numPolys;
	MPointArray pointArray;
	MIntArray polyCounts;
	MIntArray polyConnects;
	std::vector<bool> edgeSmoothing;
	MFloatArray uArray;
	MFloatArray vArray;
	MIntArray uvCounts;
	MIntArray uvIDs;
	int numUVSets;
	MStringArray uvSetNames;
	


	void appendArrayData(const DLMeshData& dataToAppend);

};



struct DLTransformData
{
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


