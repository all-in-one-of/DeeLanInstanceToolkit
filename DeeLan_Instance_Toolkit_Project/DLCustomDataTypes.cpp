#include "DLCustomDataTypes.h"



void DLMeshData::appendArrayData(const DLMeshData& dataToAppend)
{

	unsigned int ptAryLen = dataToAppend.pointArray.length();
	unsigned int polyCountsLen = dataToAppend.polyCounts.length();
	unsigned int polyConnectsLen = dataToAppend.polyConnects.length();
	size_t edgeSmoothLen = dataToAppend.edgeSmoothing.size();
	unsigned int uAryLen = dataToAppend.uArray.length();
	unsigned int vAryLen = dataToAppend.vArray.length();
	unsigned int uvIDsLen = dataToAppend.uvIDs.length();
	unsigned int uvCountsLen = dataToAppend.uvCounts.length();
	unsigned int uvSetNamesLen = dataToAppend.uvSetNames.length();

	unsigned int currentNumPoints = pointArray.length();



	for (unsigned int p = 0; p < ptAryLen; ++p)
	{
		pointArray.append(dataToAppend.pointArray[p]);
	}

	for (unsigned int p = 0; p < polyCountsLen; ++p)
	{
		polyCounts.append(dataToAppend.polyCounts[p]);
	}

	for (unsigned int p = 0; p < polyConnectsLen; ++p)
	{
		polyConnects.append(dataToAppend.polyConnects[p] + currentNumPoints);
	}

	for (size_t p = 0; p < edgeSmoothLen; ++p)
	{
		edgeSmoothing.push_back(dataToAppend.edgeSmoothing[p]);
	}

	for (unsigned int p = 0; p < uAryLen; ++p)
	{
		uArray.append(dataToAppend.uArray[p]);
	}

	for (unsigned int p = 0; p < vAryLen; ++p)
	{
		vArray.append(dataToAppend.vArray[p]);
	}

	for (unsigned int p = 0; p < uvCountsLen; ++p)
	{
		uvCounts.append(dataToAppend.uvCounts[p]);
	}

	for (unsigned int p = 0; p < uvIDsLen; ++p)
	{
		uvIDs.append(dataToAppend.uvIDs[p]);
	}

	for (unsigned int p = 0; p < uvSetNamesLen; ++p)
	{
		uvSetNames.append(dataToAppend.uvSetNames[p]);
	}

}


// DLReferenceCurveData METHODS AND DATA //

const MString DLReferenceCurveData::dataName = "dlReferenceCurveData";
const MTypeId DLReferenceCurveData::id(0x00000124);

DLReferenceCurveData::DLReferenceCurveData()
{
}

DLReferenceCurveData::~DLReferenceCurveData()
{
}

void * DLReferenceCurveData::creator()
{
	return new DLReferenceCurveData;
}

void DLReferenceCurveData::copy(const MPxData & otherCurve)
{
	points_ = ((DLReferenceCurveData&)otherCurve).points();
	normals_ = ((DLReferenceCurveData&)otherCurve).normals();

}

MTypeId DLReferenceCurveData::typeId() const
{
	return id;
}

MString DLReferenceCurveData::name() const
{
	return dataName;
}

MPointArray DLReferenceCurveData::points()
{
	return points_;
}

MVectorArray DLReferenceCurveData::normals()
{
	return normals_;
}

void DLReferenceCurveData::setPoints(const MPointArray & points)
{
	points_.clear();
	points_ = points;
}

void DLReferenceCurveData::setNormals(const MVectorArray & normals)
{
	normals_.clear();
	normals_ = normals;
}
