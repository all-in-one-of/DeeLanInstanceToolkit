#include "DLCreateInstancerCmd.h"

// COMMAND FLAGS //
#define cfNameShort "-n"
#define cfNameLong "-name"
#define cfCreateBaseMeshesShort "-cbm"
#define cfCreateBaseMeshesLong "-createBaseMeshes"
#define cfAutoUVUpdatesShort "-auu"
#define cfAutoUVUpdatesLong "-autoUVUpdates"
#define cfAutoMaterialUpdatesShort "-amu"
#define cfAutoMaterialUpdatesLong "-autoMaterialUpdates"


const MString DLCreateInstancerCmd::cmdName = "dlCreateInstancer";

DLCreateInstancerCmd::DLCreateInstancerCmd() : name_("DLInstancer_#"), autoUVUpdates_(false), 
												autoMaterialUpdates_(false), createBaseMeshes_(false)
{
}

DLCreateInstancerCmd::~DLCreateInstancerCmd()
{
}

void * DLCreateInstancerCmd::creator()
{
	return new DLCreateInstancerCmd;
}

MSyntax DLCreateInstancerCmd::initializeSyntax()
{
	MSyntax syntax;

	syntax.addFlag(cfNameShort, cfNameLong, MSyntax::kString);
	syntax.addFlag(cfCreateBaseMeshesShort, cfCreateBaseMeshesLong);
	syntax.addFlag(cfAutoUVUpdatesShort, cfAutoUVUpdatesLong);
	syntax.addFlag(cfAutoMaterialUpdatesShort, cfAutoMaterialUpdatesLong);
	syntax.setObjectType(MSyntax::kSelectionList, 0, 2);
	syntax.useSelectionAsDefault(true);

	return syntax;
}

MStatus DLCreateInstancerCmd::doIt(const MArgList & args)
{
	MStatus status;

	status = dlParseArgs_(args);
	
	if (selectionList_.length() != 2 && !createBaseMeshes_)
	{
		MGlobal::displayError("Problem Creating Instancer Node.\nPlease either select an instance object follow by a reference object, or set the \'createBaseMeshes\' flag to \'true\'");
		return MS::kSuccess;
	}

	if (createBaseMeshes_)
	{
		instanceMesh_ = dlCreateCube_();
		referenceMesh_ = dlCreatePlane_();
	}
	else
	{
		MDagPath instanceMeshPath;
		MDagPath referenceMeshPath;

		selectionList_.getDagPath(0, instanceMeshPath);
		selectionList_.getDagPath(1, referenceMeshPath);

		if (dlIsShapeNode(instanceMeshPath))
		{
			instanceMeshPath.pop();
		}

		if (dlIsShapeNode(referenceMeshPath))
		{
			referenceMeshPath.pop();
		}

		status = dlGetShapeNode_(instanceMeshPath);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		status = dlGetShapeNode_(referenceMeshPath);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		instanceMesh_ = instanceMeshPath.node();
		referenceMesh_ = referenceMeshPath.node();

	}


	// CONNECT INSTANCE AND REFERENCE MESHES TO INSTANCER NODE //
	MObject instancerNode = dgMod_.createNode(DLInstancer::id);

	MPlug instMeshPlugOutMesh = MFnDagNode(instanceMesh_).findPlug("outMesh", false);
	MPlug instMeshPlugWorldMatrix = MFnDagNode(instanceMesh_).findPlug("worldMatrix", false).elementByLogicalIndex(0, &status);

	MPlug nodePlugInstInMesh = MFnDependencyNode(instancerNode).findPlug(DLInstancer::aInstanceMesh, false);
	MPlug nodePlugInstInMatrix = MFnDependencyNode(instancerNode).findPlug(DLInstancer::aInstanceMatrix, false);

	dgMod_.connect(instMeshPlugOutMesh, nodePlugInstInMesh);
	dgMod_.connect(instMeshPlugWorldMatrix, nodePlugInstInMatrix);



	MPlug refMeshPlugOutMesh = MFnDagNode(referenceMesh_).findPlug("outMesh", false);
	MPlug refMeshPlugWorldMatrix = MFnDagNode(referenceMesh_).findPlug("worldMatrix", false).elementByLogicalIndex(0, &status);

	MPlug nodePlugNodeInMesh = MFnDependencyNode(instancerNode).findPlug(DLInstancer::aReferenceMesh, false);
	MPlug nodePlugNodeInMatrix = MFnDependencyNode(instancerNode).findPlug(DLInstancer::aReferenceMatrix, false);

	dgMod_.connect(refMeshPlugOutMesh, nodePlugNodeInMesh);
	status = dgMod_.connect(refMeshPlugWorldMatrix, nodePlugNodeInMatrix);


	// CREATE AND CONNECT OUTPUT MESH SHAPE TO INSTANCER NODE //
	MObject outputTransform = dagMod_.createNode(MString("transform"));
	dagMod_.renameNode(outputTransform, "dlInstanceObject#");

	MObject outputShape = dagMod_.createNode(MString("mesh"), outputTransform);
	dagMod_.renameNode(outputShape, "dlInstanceObjectShape#");

	MPlug instancerOutMesh = MFnDependencyNode(instancerNode).findPlug("outMesh", false);
	MPlug outputShapeInMesh = MFnDependencyNode(outputShape).findPlug("inMesh", false);

	dgMod_.connect(instancerOutMesh, outputShapeInMesh);

	redoIt();
	return MS::kSuccess;
}

MStatus DLCreateInstancerCmd::redoIt()
{
	dagMod_.doIt();
	dgMod_.doIt();
	return MS::kSuccess;
}

MStatus DLCreateInstancerCmd::undoIt()
{
	dgMod_.undoIt();
	dagMod_.undoIt();
	return MS::kSuccess;
}

bool DLCreateInstancerCmd::isUndoable() const
{
	return true;
}

MStatus DLCreateInstancerCmd::dlParseArgs_(const MArgList & args)
{
	MStatus status;

	MArgDatabase argData(syntax(), args, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	argData.getObjects(selectionList_);

	if (argData.isFlagSet(cfNameShort))
	{
		name_ = argData.flagArgumentString(cfNameShort, 0, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);
	}
	if (argData.isFlagSet(cfCreateBaseMeshesShort))
	{
		createBaseMeshes_ = true;
		CHECK_MSTATUS_AND_RETURN_IT(status);
	}
	if (argData.isFlagSet(cfAutoUVUpdatesShort))
	{
		autoUVUpdates_ = true;
		CHECK_MSTATUS_AND_RETURN_IT(status);
	}
	if (argData.isFlagSet(cfAutoMaterialUpdatesShort))
	{
		autoMaterialUpdates_ = true;
		CHECK_MSTATUS_AND_RETURN_IT(status);
	}

	return MS::kSuccess;
}

MObject DLCreateInstancerCmd::dlCreateCube_()
{

	MObject cubeTransform = dagMod_.createNode(MString("transform"));
	dagMod_.renameNode(cubeTransform, "dlInstancerCube#");

	MObject cubeShape = dagMod_.createNode(MString("mesh"), cubeTransform);
	dagMod_.renameNode(cubeShape, "dlInstancerCubeShape#");

	MObject cubeCreator = dgMod_.createNode(MString("polyCube"));

	MPlug cubeCreatorOutput = MFnDependencyNode(cubeCreator).findPlug("output", false);
	MPlug cubeShapeInMesh = MFnDependencyNode(cubeShape).findPlug("inMesh", false);

	dgMod_.connect(cubeCreatorOutput, cubeShapeInMesh);

	return cubeShape;
}

MObject DLCreateInstancerCmd::dlCreatePlane_()
{

	MObject planeTransform = dagMod_.createNode(MString("transform"));
	dagMod_.renameNode(planeTransform, "dlInstancerPlane#");

	MObject planeShape = dagMod_.createNode(MString("mesh"), planeTransform);
	dagMod_.renameNode(planeShape, "dlInstancerPlaneShape#");

	MObject planeCreator = dgMod_.createNode(MString("polyPlane"));

	MPlug planeCreatorOutput = MFnDependencyNode(planeCreator).findPlug("output", false);
	MPlug planeShapeInMesh = MFnDependencyNode(planeShape).findPlug("inMesh", false);

	dgMod_.connect(planeCreatorOutput, planeShapeInMesh);

	return planeShape;
}

bool DLCreateInstancerCmd::dlIsShapeNode(const MDagPath & path)
{
	return path.node().hasFn(MFn::kMesh) || path.node().hasFn(MFn::kNurbsCurve) || 
			path.node().hasFn(MFn::kNurbsSurface);
}

MStatus DLCreateInstancerCmd::dlGetShapeNode_(MDagPath & path, bool intermediate)
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
