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
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshFaceVertex.h>
#include <maya/MItDependencyGraph.h>
#include <maya/MFloatArray.h>
#include <maya/MMatrixArray.h>
#include <maya/MFnMesh.h>
#include <maya/MFnMeshData.h>
#include <maya/MGlobal.h>

#include <map>
#include <vector>

#include "DLCustomDataTypes.h"


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

	MStatus dlGetMeshData(const MObject& mesh, DLMeshData& meshData);

	MStatus dlCreateOutputMeshData(const DLMeshData& inMeshData, unsigned int numCopies,
								DLMeshData& outMeshData, bool clearData = true);

	MObject dlCreateMesh(const DLMeshData& meshData);

	MStatus dlCalculateVectorAngles(float3 base, float3 direction, float3& angles);

	MStatus dlGenerateRandomValues(float3 maxDifference, float3& values);

	MMatrixArray dlGenerateMatricies(const DLTransformData& transformData);

	MStatus dlDeformMesh(MObject& mesh, MMatrixArray& matricies);


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
	DLMeshData inputInstanceMeshData_;
	DLMeshData outputInstanceMeshData_;
	DLTransformData transformData_;
	MMatrixArray ouputTransformMatricies_;
	MMatrixArray previousTransformMatricies_;
	unsigned int numInstances_;
	std::map<attrs, bool> attributeDirty_;
};

