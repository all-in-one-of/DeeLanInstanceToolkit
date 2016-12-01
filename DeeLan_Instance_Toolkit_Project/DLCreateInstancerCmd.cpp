#include "DLCreateInstancerCmd.h"

// COMMAND FLAGS //
#define cfNameShort "-n"
#define cfNameLong "-name"
#define cfCreateBaseMeshesShort "-cbm"
#define cfCreateBaseMeshesLong "-createBaseMeshes"


const MString DLCreateInstancerCmd::cmdName = "dlCreateInstancer";

DLCreateInstancerCmd::DLCreateInstancerCmd() : name_("DLInstancer_#"), createBaseMeshes_(false)
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
		instanceMesh_ = dlCreateObject_(kCube, MString("dlInstancerCube"));
		referenceMesh_ = dlCreateObject_(kPlane, MString("dlInstancerPlane"));
	}
	else
	{
		MDagPath instanceMeshPath;
		MDagPath referenceMeshPath;

		selectionList_.getDagPath(0, instanceMeshPath);
		selectionList_.getDagPath(1, referenceMeshPath);

		if (DLCommon::dlIsShapeNode(instanceMeshPath))
		{
			instanceMeshPath.pop();
		}

		if (DLCommon::dlIsShapeNode(referenceMeshPath))
		{
			referenceMeshPath.pop();
		}

		status = DLCommon::dlGetShapeNode(instanceMeshPath);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		status = DLCommon::dlGetShapeNode(referenceMeshPath);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		instanceMesh_ = instanceMeshPath.node();
		referenceMesh_ = referenceMeshPath.node();

	}


	// CONNECT INSTANCE AND REFERENCE MESHES TO INSTANCER NODE //
	MObject instancerNode = dgMod_.createNode(DLInstancer::id);
	MObject outputShape = dlCreateObject_(kNull, MString("dlInstancedObject"));

	MFnDagNode instMeshDGNode(instanceMesh_);
	MFnDagNode refMeshDGNode(referenceMesh_);
	MFnDependencyNode instancerDGNode(instancerNode);
	MFnDependencyNode outputShapeDGNode(outputShape);

	MPlug instMeshPlugMessage = instMeshDGNode.findPlug("message", false);
	MPlug instMeshPlugOutMesh = instMeshDGNode.findPlug("outMesh", false);
	MPlug instMeshPlugWorldMatrix = instMeshDGNode.findPlug("worldMatrix", false).elementByLogicalIndex(0, &status);

	MPlug nodePlugInstMeshMessage = instancerDGNode.findPlug(DLInstancer::aInstanceMessage, false);
	MPlug nodePlugInstInMesh = instancerDGNode.findPlug(DLInstancer::aInstanceMesh, false);
	MPlug nodePlugInstInMatrix = instancerDGNode.findPlug(DLInstancer::aInstanceMatrix, false);

	dgMod_.connect(instMeshPlugMessage, nodePlugInstMeshMessage);
	dgMod_.connect(instMeshPlugOutMesh, nodePlugInstInMesh);
	dgMod_.connect(instMeshPlugWorldMatrix, nodePlugInstInMatrix);



	MPlug refMeshPlugOutMesh = refMeshDGNode.findPlug("outMesh", false);
	MPlug refMeshPlugWorldMatrix = refMeshDGNode.findPlug("worldMatrix", false).elementByLogicalIndex(0, &status);

	MPlug nodePlugNodeInMesh = instancerDGNode.findPlug(DLInstancer::aReferenceMesh, false);
	MPlug nodePlugNodeInMatrix = instancerDGNode.findPlug(DLInstancer::aReferenceMatrix, false);

	dgMod_.connect(refMeshPlugOutMesh, nodePlugNodeInMesh);
	status = dgMod_.connect(refMeshPlugWorldMatrix, nodePlugNodeInMatrix);


	// CREATE AND CONNECT OUTPUT MESH SHAPE TO INSTANCER NODE //
	
	MPlug instancerOutMesh = instancerDGNode.findPlug("outMesh", false);
	MPlug outputShapeInMesh = outputShapeDGNode.findPlug("inMesh", false);
	dgMod_.connect(instancerOutMesh, outputShapeInMesh);



	MPlug instancerOutputMeshMessage = instancerDGNode.findPlug(DLInstancer::aOutputMeshNodeMessage, false);
	MPlug outputShapeMessage = outputShapeDGNode.findPlug("message", false);
	dgMod_.connect(outputShapeMessage, instancerOutputMeshMessage);


	//MGlobal::displayInfo("TEST");
	MPlug outputMeshPlugInstObjGroups0 = outputShapeDGNode.findPlug("instObjGroups", false).elementByLogicalIndex(0 &status);
	//MGlobal::displayInfo(status.errorString());
	MPlug instanceMeshPlugInstObjGroups0 = instMeshDGNode.findPlug("instObjGroups", false).elementByLogicalIndex(0 & status);
	//MGlobal::displayInfo(status.errorString());

	MPlugArray connectedShadingPlugs;
	instanceMeshPlugInstObjGroups0.connectedTo(connectedShadingPlugs, false, true, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	if (connectedShadingPlugs.length() != 0 || !createBaseMeshes_)
	{
			
		MPlug outputMeshShaderPlug;
		MPlug tempPlug;
		DLCommon::dlGetMaterialConnectionPlugs(instMeshDGNode, tempPlug, outputMeshShaderPlug);
		dgMod_.connect(outputMeshPlugInstObjGroups0, outputMeshShaderPlug);
	}
	else if (createBaseMeshes_)
	{
		MPlug instObjGroupsIndex0 = outputShapeDGNode.findPlug("instObjGroups", false).elementByLogicalIndex(0);
		MPlug defaultShaderPlug = dlGetDefaultShaderPlug_();
		dgMod_.connect(instObjGroupsIndex0, defaultShaderPlug);

	}

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


	return MS::kSuccess;
}

MObject DLCreateInstancerCmd::dlCreateObject_(dlObjectType type, MString & name)
{
	MString transformName = name + "#";;
	MString shapeName = name + "Shape#";;
	bool addObjectCreator = false;
	MString creatorType;

	if (type == kNull)
	{

	}
	else if (type == kCube)
	{
		addObjectCreator = true;
		creatorType = "polyCube";
	}
	else if (type == kPlane)
	{
		addObjectCreator = true;
		creatorType = "polyPlane";
	}


	MObject transformNode = dagMod_.createNode(MString("transform"));
	dagMod_.renameNode(transformNode, transformName);

	MObject shapeNode = dagMod_.createNode(MString("mesh"), transformNode);
	dagMod_.renameNode(shapeNode, shapeName);

	if (addObjectCreator)
	{
		MStatus status;
		MObject creatorObj = dgMod_.createNode(creatorType);
		MFnDependencyNode creatorNode(creatorObj);

		MPlug creatorOutput = creatorNode.findPlug("output", false);
		MPlug shapeInMesh = MFnDependencyNode(shapeNode).findPlug("inMesh", false);

		dgMod_.connect(creatorOutput, shapeInMesh);

		MPlug instObjGroupsIndex0 = MFnDependencyNode(shapeNode).findPlug("instObjGroups", false).elementByLogicalIndex(0);
		
		MPlug dagSetMembersParentChild = dlGetDefaultShaderPlug_();
		dgMod_.connect(instObjGroupsIndex0, dagSetMembersParentChild);

		if (type == kCube)
		{
			double edgeLength = 0.5;

			MPlug height = creatorNode.findPlug("height", false);
			MPlug depth = creatorNode.findPlug("depth", false);
			MPlug width = creatorNode.findPlug("width", false);

			height.setDouble(edgeLength);
			depth.setDouble(edgeLength);
			width.setDouble(edgeLength);
		}
		else if (type == kPlane)
		{
			double edgeLength = 10;

			MPlug height = creatorNode.findPlug("height", false);
			MPlug width = creatorNode.findPlug("width", false);

			height.setDouble(edgeLength);
			width.setDouble(edgeLength);
		}
	}

	
	return shapeNode;
}

MPlug DLCreateInstancerCmd::dlGetDefaultShaderPlug_()
{
	MObject initialShadingGroup;
	MItDependencyNodes itDepNodes(MFn::kShadingEngine);
	for (; !itDepNodes.isDone(); itDepNodes.next())
	{
		MFnDependencyNode currentNode(itDepNodes.thisNode());
		if (currentNode.name() == MString("initialShadingGroup"))
		{
			initialShadingGroup = itDepNodes.thisNode();
			break;

		}
	}

	MPlug dagSetMembersParent = MFnDependencyNode(initialShadingGroup).findPlug("dagSetMembers", false);
	unsigned int numConnectedMeshes = dagSetMembersParent.numConnectedElements();
	unsigned int nextFreePlugByLogicalIndex = dagSetMembersParent.elementByPhysicalIndex(numConnectedMeshes).logicalIndex();
	MPlug returnPlug = dagSetMembersParent.elementByLogicalIndex(nextFreePlugByLogicalIndex);

	return returnPlug;

}
