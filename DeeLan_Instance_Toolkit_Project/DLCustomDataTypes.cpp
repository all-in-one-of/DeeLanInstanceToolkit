#include "DLCustomDataTypes.h"



void DLMeshData::appendArrayData(const DLMeshData& dataToAppend)
{

	unsigned int ptAryLen = dataToAppend.pointArray.length();
	unsigned int polyCountsLen = dataToAppend.polyCounts.length();
	unsigned int polyConnectsLen = dataToAppend.polyConnects.length();
	unsigned int normalsLen = dataToAppend.normals.length();
	unsigned int uAryLen = dataToAppend.uArray.length();
	unsigned int vAryLen = dataToAppend.vArray.length();
	unsigned int uvIDsLen = dataToAppend.uvIDs.length();
	unsigned int uvCountsLen = dataToAppend.uvCounts.length();

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

	for (unsigned int p = 0; p < normalsLen; ++p)
	{
		normals.append(dataToAppend.normals[p]);
	}

	for (unsigned int p = 0; p < uAryLen; ++p)
	{
		uArray.append(dataToAppend.uArray[p]);
	}

	for (unsigned int p = 0; p < vAryLen; ++p)
	{
		vArray.append(dataToAppend.vArray[p]);
	}

	for (unsigned int p = 0; p < uvIDsLen; ++p)
	{
		uvIDs.append(dataToAppend.uvIDs[p]);
	}

	for (unsigned int p = 0; p < uvCountsLen; ++p)
	{
		uvCounts.append(dataToAppend.uvCounts[p]);
	}

}
