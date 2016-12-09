#pragma once

#pragma once

#include <maya/MPxNode.h>
#include <maya/MGlobal.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MVector.h>
#include <maya/MVectorArray.h>
#include <maya/MMatrix.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MDataBlock.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnGenericAttribute.h>


#include "DLCustomDataTypes.h"
#include "DLCommon.h"


class DLCurveToReference : public MPxNode
{
public:
	DLCurveToReference();
	virtual ~DLCurveToReference();
	static void* creator();
	static MStatus initialize();

	virtual MStatus compute(const MPlug& plug, MDataBlock& data);


	//Name and ID
	static MTypeId id;
	static const MString nodeName;

	//Input Attributes
	static MObject aInputCurveData;
	static MObject aInputNurbs;
	static MObject aInputMatrix;

	static MObject aNumInstances;

	//Output Attributes
	static MObject aOutCurveData;


private:
	int numInstances_;
	MPointArray referencePoints_;
	MVectorArray referenceNormals_;

	MStatus dlGenerateReferencePointsOnCurve_(MFnNurbsCurve& fnCurve);
};

