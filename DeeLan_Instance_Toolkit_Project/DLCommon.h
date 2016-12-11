#pragma once

#include <maya/MGlobal.h>
#include <maya/MVector.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MDagPath.h>

#include <maya/MFnDagNode.h>
#include <maya/MItDependencyGraph.h>

#include <random>

class DLCommon
{
public:

	//ENUMS
	enum DLRandomMode { kNormal, kTranslate, kRotate, kScale, kCurveParam };

	//METHODS

	static float dlGenerateRandomValues(int inSeed, float maxDifference, DLRandomMode mode);

	static MVector dlGenerateRandomValues(int inSeed, float3 maxDifference, DLRandomMode mode);

	static MStatus dlGetMaterialConnectionPlugs(const MFnDependencyNode& sourceNode,
							MPlug& currentMaterialPlug, MPlug& nextAvailablePlug);

	static bool dlIsShapeNode(const MDagPath& path);

	static MStatus dlGetShapeNode(MDagPath& path, bool intermediate = false);
private:
	static int getModeSeed(DLRandomMode mode);
};
