#include "DLCurveToReference.h"

MTypeId DLCurveToReference::id(0x00000024);
const MString DLCurveToReference::nodeName = "dlCurveToReference";

MObject DLCurveToReference::aInputCurveData;
MObject DLCurveToReference::aInputNurbs;
MObject DLCurveToReference::aInputMatrix;
MObject DLCurveToReference::aNumInstances;

MObject DLCurveToReference::aOutCurveData;


DLCurveToReference::DLCurveToReference() : numInstances_(3)
{
}

DLCurveToReference::~DLCurveToReference()
{
}

void * DLCurveToReference::creator()
{
	return new DLCurveToReference;
}

MStatus DLCurveToReference::initialize()
{
	MStatus status;

	MFnCompoundAttribute cAttr;
	MFnTypedAttribute tAttr;
	MFnMatrixAttribute mAttr;
	MFnNumericAttribute nAttr;
	MFnGenericAttribute gAttr;

	// OUTPUT ATTRIBUTE //
	aOutCurveData = gAttr.create("outCurveData", "outData");
	gAttr.addAccept(DLReferenceCurveData::id);
	gAttr.setWritable(false);
	gAttr.setStorable(false);
	addAttribute(aOutCurveData);

	// INPUT ATTRIBUTE //
	aInputCurveData = cAttr.create("inputCurveData", "inData");
	cAttr.setReadable(false);

	aInputNurbs = tAttr.create("inputNurbsCurve", "inCurve", MFnData::kNurbsCurve);
	tAttr.setReadable(false);
	

	aInputMatrix = mAttr.create("inputCurveMatrix", "inMatrix");
	mAttr.setReadable(false);

	cAttr.addChild(aInputNurbs);
	cAttr.addChild(aInputMatrix);

	addAttribute(aInputCurveData);
	attributeAffects(aInputNurbs, aOutCurveData);
	attributeAffects(aInputMatrix, aOutCurveData);



	aNumInstances = nAttr.create("numberOfInstances", "numInstances", MFnNumericData::kInt, 6);
	nAttr.setMin(1);
	nAttr.setKeyable(true);
	addAttribute(aNumInstances);
	attributeAffects(aNumInstances, aOutCurveData);



	return MS::kSuccess;
}


MStatus DLCurveToReference::compute(const MPlug & plug, MDataBlock & data)
{
	MStatus status;

	if (plug != aOutCurveData)
	{
		return MS::kUnknownParameter;
	}

	numInstances_ = data.inputValue(aNumInstances, &status).asInt();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	MObject oCurve = data.inputValue(aInputNurbs, &status).asNurbsCurve();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	MMatrix worldMatrix = data.inputValue(aInputMatrix, &status).asMatrix();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	MFnNurbsCurve fnCurve(oCurve, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = dlGenerateReferencePointsOnCurve_(fnCurve);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	for (int i = 0; i < numInstances_; i++)
	{
		referencePoints_[i] *= worldMatrix;
		referenceNormals_[i] *= worldMatrix;
	}
	return MS::kSuccess;
}


MStatus DLCurveToReference::dlGenerateReferencePointsOnCurve_(MFnNurbsCurve & fnCurve)
{
	MStatus status;

	referencePoints_.clear();
	referenceNormals_.clear();


	double curveLength = fnCurve.length();
	
	if (numInstances_ == 1)
	{
		double param = fnCurve.findParamFromLength(curveLength / 2);
		MPoint point;
		MVector normal;
		status = fnCurve.getPointAtParam(param, point);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		normal = fnCurve.normal(param, MSpace::kObject, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		status = referencePoints_.append(point);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		status = referenceNormals_.append(normal);
		CHECK_MSTATUS_AND_RETURN_IT(status);
	}
	else
	{
		double param;
		MPoint point;
		MVector normal;
		double segmentLength = curveLength / (numInstances_ - 1);
		for (int i = 0; i < numInstances_; i++)
		{
			param = fnCurve.findParamFromLength(segmentLength * i);
			status = fnCurve.getPointAtParam(param, point);
			CHECK_MSTATUS_AND_RETURN_IT(status);
			normal = fnCurve.normal(param, MSpace::kObject, &status);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			status = referencePoints_.append(point);
			CHECK_MSTATUS_AND_RETURN_IT(status);
			status = referenceNormals_.append(normal);
			CHECK_MSTATUS_AND_RETURN_IT(status);
		}
	}
	return MS::kSuccess;
}
