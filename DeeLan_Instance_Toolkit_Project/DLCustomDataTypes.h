#pragma once

#include <maya/MPxData.h>
#include <maya/MIntArray.h>
#include <maya/MPointArray.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MVectorArray.h>
#include <maya/MMatrixArray.h>
#include <maya/MTypeId.h>
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




class DLReferenceCurveData : public MPxData
{
public:
	DLReferenceCurveData();
	~DLReferenceCurveData();

	static void* creator();
	virtual void copy(const MPxData& otherCurve);
	MTypeId typeId() const;
	MString name() const;

	MPointArray points();
	MVectorArray normals();
	void setPoints(const MPointArray& points);
	void setNormals(const MVectorArray& normals);

	static const MString dataName;
	static const MTypeId id;

private:
	MPointArray points_;
	MVectorArray normals_;
};