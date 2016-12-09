#include "DLCommon.h"

int DLCommon::getModeSeed(DLRandomMode mode)
{
	int seed;

	if (mode == kNormal)
	{
		seed = 1234;
	}
	else if (mode == kTranslate)
	{
		seed = 2143;
	}
	else if (mode == kRotate)
	{
		seed = 3241;
	}
	else if (mode == kScale)
	{
		seed = 4312;
	}

	return seed;
}




float DLCommon::dlGenerateRandomValues(int inSeed, float maxDifference, DLRandomMode mode)
{
	int modeSeed = getModeSeed(mode);
	int seed = inSeed + modeSeed;
	std::mt19937 randGen(seed);
	
	float deviation = maxDifference / 2;
	std::uniform_real_distribution<float> offset(-deviation, deviation);
	float outFloat = offset(randGen);
	return outFloat;
}

MVector DLCommon::dlGenerateRandomValues(int inSeed, float3 maxDifference, DLRandomMode mode)
{
	int modeSeed = getModeSeed(mode);
	MVector outVec;

	for (int i = 0; i < 3; ++i)
	{
		int indexSeed = i * 13;
		int seed = inSeed + modeSeed + indexSeed;
		std::mt19937 randGen(seed);

		float deviation = maxDifference[i] / 2;
		std::uniform_real_distribution<float> offset(-deviation, deviation);

		outVec[i] = offset(randGen);
	}
	return outVec;
}

MStatus DLCommon::dlGetMaterialConnectionPlugs(const MFnDependencyNode& sourceNode, 
												MPlug& currentMaterialPlug,  MPlug& nextAvailablePlug)
{
	MStatus status;

	MPlug instObjGroupsIndex0 = sourceNode.findPlug("instObjGroups", false, &status).elementByLogicalIndex(0);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	if (instObjGroupsIndex0.isConnected())
	{
		MPlugArray connectedPlugs;
		instObjGroupsIndex0.connectedTo(connectedPlugs, false, true, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		if (connectedPlugs.length() != 0)
		{
			currentMaterialPlug = connectedPlugs[0];
			MPlug shaderConnectionParent = currentMaterialPlug.array();

			unsigned int numConnectedMeshes = shaderConnectionParent.numConnectedElements();
			unsigned int nextFreePlugByLogicalIndex = shaderConnectionParent.elementByPhysicalIndex(numConnectedMeshes).logicalIndex();
			nextAvailablePlug = shaderConnectionParent.elementByLogicalIndex(nextFreePlugByLogicalIndex);
			return MS::kSuccess;

		}
	}
	return MS::kFailure;
}

bool DLCommon::dlIsShapeNode(const MDagPath & path)
{
	return path.node().hasFn(MFn::kMesh) || path.node().hasFn(MFn::kNurbsCurve) ||
		path.node().hasFn(MFn::kNurbsSurface);
}

MStatus DLCommon::dlGetShapeNode(MDagPath & path, bool intermediate)
{
	MStatus status;

	bool suitableShapeFound;
	unsigned int numShapes;
	path.numberOfShapesDirectlyBelow(numShapes);

	for (unsigned int i = 0; i < numShapes; ++i)
	{
		path.extendToShapeDirectlyBelow(i);
		MFnDagNode node(path);
		if (node.isIntermediateObject() == intermediate)
		{
			suitableShapeFound = true;
			break;
		}
		path.pop();
	}

	if (suitableShapeFound)
	{
		return MS::kSuccess;
	}
	else
	{
		if (!intermediate)
		{
			MGlobal::displayError("NO SHAPE FOUND");
		}
		else
		{
			MGlobal::displayError("NO INTERMEDIATE SHAPES FOUND");
		}
		return MS::kFailure;
	}
}

