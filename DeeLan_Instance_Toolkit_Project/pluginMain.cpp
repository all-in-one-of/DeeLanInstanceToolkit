
#include <maya/MFnPlugin.h>

#include "DLInstancer.h"
#include "DLCreateInstancerCmd.h"
#include "DLUpdateMaterialsCmd.h"

MStatus initializePlugin(MObject obj)
{
	MStatus status;

	MFnPlugin fnPlugin(obj, "DeeLan", "0.1", "Any");
	status = fnPlugin.registerNode(DLInstancer::nodeName, DLInstancer::id, 
		DLInstancer::creator, DLInstancer::initialize);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = fnPlugin.registerCommand(DLCreateInstancerCmd::cmdName, DLCreateInstancerCmd::creator, DLCreateInstancerCmd::initializeSyntax);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = fnPlugin.registerCommand(DLUpdateMaterialsCmd::cmdName, DLUpdateMaterialsCmd::creator, DLUpdateMaterialsCmd::initializeSyntax);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	return MS::kSuccess;
}


MStatus uninitializePlugin(MObject obj)
{
	MStatus status;

	MFnPlugin fnPlugin(obj);
	status = fnPlugin.deregisterNode(DLInstancer::id);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = fnPlugin.deregisterCommand(DLCreateInstancerCmd::cmdName);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = fnPlugin.deregisterCommand(DLUpdateMaterialsCmd::cmdName);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	return MS::kSuccess;
}

//this is a quick comment to test merging in Github

