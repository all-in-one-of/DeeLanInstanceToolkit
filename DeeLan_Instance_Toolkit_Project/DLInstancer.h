#pragma once

#include <maya/MPxNode.h>
#include <maya/MPxGeometryFilter.h>
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
#include <maya/MItGeometry.h>
#include <maya/MFloatArray.h>
#include <maya/MMatrixArray.h>
#include <maya/MFnMesh.h>
#include <maya/MFnMeshData.h>
#include <maya/MGlobal.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MTime.h>
#include <maya/MAnimControl.h>

#include <map>
#include <vector>

#include "DLCustomDataTypes.h"
#include "DLCommon.h"


class DLInstancer : public MPxNode
{
public:
	DLInstancer();
	virtual ~DLInstancer();
	static void* creator();
	static MStatus initialize();

	virtual MStatus setDependentsDirty(const MPlug&, MPlugArray&);

	virtual MStatus compute(const MPlug& plug, MDataBlock& data);

	MStatus dlManualSetDependentsDirty(MDataBlock& data);

	MStatus dlGetMeshData(const MObject& mesh, DLMeshData& meshData);

	MStatus dlCreateOutputMeshData(const DLMeshData& inMeshData, unsigned int numCopies,
								DLMeshData& outMeshData, bool clearData = true);

	MObject dlCreateMesh(const DLMeshData& meshData);

	MStatus dlCalculateVectorAngles(float3 base, float3 direction, float3& angles);

	MMatrixArray dlGenerateMatricies(const DLTransformData& transformData);

	MStatus dlDeformMesh(MDataHandle& meshDataHandle, MMatrixArray& matricies);


	enum attrs
	{ kInstanceMesh, kReferenceMesh, kOffsets, kRandoms };

	//Name and ID
	static MTypeId id;
	static const MString nodeName;

	//Input Attributes
	static MObject aInstanceMesh;
	static MObject aReferenceMesh;
	static MObject aNormalOffset;
	static MObject aTranslateOffset;
	static MObject aRotationOffset;
	static MObject aUniformScaleOffset;
	static MObject aScaleOffset;
	static MObject aNormalRandom;
	static MObject aTranslateRandom;
	static MObject aRotationRandom;
	static MObject aUniformScaleRandom;
	static MObject aScaleRandom;
	static MObject aNodeSeed;
	static MObject aGeneratedMesh;
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
	unsigned int numInstances_;
	unsigned int numInstanceMeshPoints_;
	std::map<attrs, bool> attributeDirty_;
	MTime prevTime_;
	bool setDependentsDirtyCalled_;
	
};

