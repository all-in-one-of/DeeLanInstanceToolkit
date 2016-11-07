#pragma once
#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MPointArray.h>
#include <maya/MPoint.h>
#include <maya/MIntArray.h>
#include <maya/MFloatArray.h>
#include <maya/MMatrixArray.h>

#include <map>

struct DLMeshData
{
	unsigned int numPoints;
	unsigned int numPolys;
	MPointArray pointArray;
	MIntArray polyCounts;
	MIntArray polyConnects;
	MFloatArray uArray;
	MFloatArray vArray;
	MIntArray uvIDs;
	MIntArray uvCounts;
};



class DLInstancer : public MPxNode
{
public:
	DLInstancer();
	virtual ~DLInstancer();

	static void* creator();
	static MStatus initialize();

	virtual MStatus setDependentsDirty(const MPlug&, MPlugArray&);


	/// <param name="plug">Plug to be recalculated.</param> 
	/// <param name="data">Rhe data block.</param> 
	virtual MStatus compute(const MPlug &plug, MDataBlock &data);


	enum attrs
	{ kInstanceMesh, kReferenceMesh, kNormalOffset, kTranslateOffset,
		kRotationOffset, kScaleOffset, kNormalRandom, kTranslateRandom, 
			kRotationRandom, kScaleRandom };

	//Name and ID
	static MTypeId id;
	static const MString nodeName;

	//Input Attributes
	static MObject aInstanceMesh;
	static MObject aReferenceMesh;
	static MObject aNormalOffset;
	static MObject aTranslateOffset;
	static MObject aRotationOffset;
	static MObject aScaleOffset;
	static MObject aNormalRandom;
	static MObject aTranslateRandom;
	static MObject aRotationRandom;
	static MObject aScaleRandom;
	//Output Attributes
	static MObject aOutMesh;
	static MObject aInstanceGroup;
	static MObject aInstanceGroupMesh;
	static MObject aInstanceGroupMatricies;

private:
	DLMeshData instanceMeshData_;
	MMatrixArray referenceMatricies_;
	unsigned int numInstances_;
	std::map<attrs, bool> attributeDirty_;
};