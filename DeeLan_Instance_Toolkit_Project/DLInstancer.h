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
#include <maya/MItMeshEdge.h>
#include <maya/MFloatArray.h>
#include <maya/MMatrixArray.h>
#include <maya/MFnMesh.h>
#include <maya/MFnMeshData.h>
#include <maya/MGlobal.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MTime.h>
#include <maya/MAnimControl.h>
#include <maya/MFloatMatrix.h>


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

	MMatrix dlGenerateNormalAlignmentMatrix(MVector direction);

	MMatrixArray dlGenerateInstanceDeformMatricies(const DLTransformData& transformData, bool alignNormals);

	MStatus dlDeformMesh(MDataHandle& meshDataHandle, MMatrixArray& matricies);


	enum attrs
	{ kInstanceMesh, kReferenceMesh, kOffsets, kRandoms, kAlignment, kTransMatrix,};

	//Name and ID
	static MTypeId id;
	static const MString nodeName;

	//Input Attributes
	static MObject aInstanceObject;
	static MObject aInstanceMesh;
	static MObject aInstanceMatrix;
	static MObject aReferenceObject;
	static MObject aReferenceMesh;
	static MObject aReferenceMatrix;

	static MObject aAlignToNormals;

	static MObject aNormalOffset;
	static MObject aTranslate;
	static MObject aRotate;
	static MObject aUniformScale;
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


private:
	DLMeshData inputMeshData_;
	DLMeshData outputMeshData_;
	DLTransformData transformData_;
	MTransformationMatrix instanceMatrix_;
	MMatrix referenceMatrix_;
	MMatrixArray ouputTransformMatricies_;
	unsigned int numInstances_;
	unsigned int numInstanceMeshPoints_;
	std::map<attrs, bool> attributeDirty_;
	MTime prevTime_;
	bool setDependentsDirtyCalled_;
};

