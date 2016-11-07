#pragma once
#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnMatrixAttribute.h>

class DLInstancer : public MPxNode
{
public:
	DLInstancer();
	virtual ~DLInstancer();

	static void* creator();
	static MStatus initialize();

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

};