#include "DLUpdateMaterialsCmd.h"

const MString DLUpdateMaterialsCmd::cmdName = "dlUpdateMaterials";


DLUpdateMaterialsCmd::DLUpdateMaterialsCmd()
{
}

DLUpdateMaterialsCmd::~DLUpdateMaterialsCmd()
{
}

void * DLUpdateMaterialsCmd::creator()
{
	return new DLUpdateMaterialsCmd;
}

MSyntax DLUpdateMaterialsCmd::initializeSyntax()
{
	MSyntax syntax;

	syntax.setObjectType(MSyntax::kSelectionList, 1, 1);
	syntax.useSelectionAsDefault(true);

	return syntax;
}

MStatus DLUpdateMaterialsCmd::doIt(const MArgList & args)
{
	MStatus status;

	status = dlParseArgs_(args);
	CHECK_MSTATUS_AND_RETURN_IT(status);



	if (selectedNode_.hasFn(MFn::kDagNode))
	{
		dlGetInstancerNode_();
		displayInfo("DAG NODE");
		return MS::kSuccess; // TESTING ONLY DELETE!!!!!
	}
	else if (MFnDependencyNode(selectedNode_).typeId() == DLInstancer::id)
	{
		instancerNode_ = selectedNode_;
		displayInfo("dlInstancerNode NODE");
	}
	else
	{
		MGlobal::displayError("Unable to update materials due to invalid selection. Please select either a dlInstancer node, or a Dag node.");
		return MS::kSuccess;
	}

	dlConnectMaterials_();

	return MS::kSuccess;
}

MStatus DLUpdateMaterialsCmd::redoIt()
{
	MStatus status;



	return MS::kSuccess;
}

MStatus DLUpdateMaterialsCmd::undoIt()
{
	MStatus status;



	return MS::kSuccess;
}

bool DLUpdateMaterialsCmd::isUndoable() const
{
	return true;
}

MStatus DLUpdateMaterialsCmd::dlParseArgs_(const MArgList & args)
{
	MStatus status;

	MArgDatabase argData(syntax(), args, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	argData.getObjects(selectionList_);

	
	status = selectionList_.getDependNode(0, selectedNode_);
	CHECK_MSTATUS_AND_RETURN_IT(status);


	return MS::kSuccess;
}

MStatus DLUpdateMaterialsCmd::dlGetInstancerNode_()
{
	MStatus status;
	


	MFnDagNode selectedDAGNode(selectedNode_, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	MGlobal::displayInfo(selectedDAGNode.name());

	MDagPath pathSelectedNode;
	status = selectedDAGNode.getPath(pathSelectedNode);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	displayInfo(pathSelectedNode.fullPathName());

	if (DLCommon::dlIsShapeNode(pathSelectedNode))
	{
		status = pathSelectedNode.pop();
		CHECK_MSTATUS_AND_RETURN_IT(status);
	}

	displayInfo(pathSelectedNode.fullPathName());

	status = DLCommon::dlGetShapeNode(pathSelectedNode);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	displayInfo(pathSelectedNode.fullPathName());


	MFnDagNode mainShapeDAGNode(pathSelectedNode.node());
	MPlug plugInMesh = mainShapeDAGNode.findPlug("inMesh", false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	MItDependencyGraph itDG(plugInMesh, MFn::kInvalid, MItDependencyGraph::kUpstream, 
							MItDependencyGraph::kBreadthFirst, MItDependencyGraph::kNodeLevel, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	for (; !itDG.isDone(); itDG.next())
	{
		MGlobal::displayInfo(itDG.currentItem().apiTypeStr());
	}

	return MS::kSuccess;
}

MStatus DLUpdateMaterialsCmd::dlConnectMaterials_()
{
	MStatus status;

	MFnDependencyNode instancerDPNode(instancerNode_);

	MGlobal::displayInfo("UPDATING MATERIALS");
	MPlugArray instanceMeshMessageConnected;
	MPlug instanceMeshMessage = instancerDPNode.findPlug("instanceMeshMessage", false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	instanceMeshMessage.connectedTo(instanceMeshMessageConnected, true, false);
	MFnDependencyNode instanceMesh(instanceMeshMessageConnected[0].node());

	MPlug currentInstanceMaterialPlug;
	MPlug nextInstanceMaterialPlug;
	status = DLCommon::dlGetMaterialConnectionPlugs(instanceMesh, currentInstanceMaterialPlug, nextInstanceMaterialPlug);
	CHECK_MSTATUS_AND_RETURN_IT(status);


	MPlugArray outputMeshMessageConnected;
	MPlug outputMeshMessage = instancerDPNode.findPlug("outputMeshNode", false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	outputMeshMessage.connectedTo(outputMeshMessageConnected, true, false);
	MFnDependencyNode outputMesh(outputMeshMessageConnected[0].node());

	MPlug currentOutputMaterialPlug;
	MPlug nextOutputMaterialPlug;
	status = DLCommon::dlGetMaterialConnectionPlugs(outputMesh, currentOutputMaterialPlug, nextOutputMaterialPlug);
	//CHECK_MSTATUS_AND_RETURN_IT(status);

	MGlobal::displayInfo(currentInstanceMaterialPlug.name());
	MGlobal::displayInfo(nextOutputMaterialPlug.name());


	if (currentInstanceMaterialPlug.node() != nextOutputMaterialPlug.node())
	{
		MDGModifier dgMod;
		MPlug ouputInstObjGroups0 = outputMesh.findPlug("instObjGroups", false).elementByLogicalIndex(0);

		status = dgMod.disconnect(ouputInstObjGroups0, currentOutputMaterialPlug);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		status = dgMod.connect(ouputInstObjGroups0, nextInstanceMaterialPlug);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		status = dgMod.doIt();
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MGlobal::displayInfo("dgMod EXECUTED");
	}

	return MS::kSuccess;
}
