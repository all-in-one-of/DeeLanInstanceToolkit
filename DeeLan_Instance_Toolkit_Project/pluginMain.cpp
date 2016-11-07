#include <maya/MFnPlugin.h>

#include "DLInstancer.h"

MStatus initializePlugin(MObject& obj)
{
	MStatus status;

	MFnPlugin fnPlugin(obj, "DeeLan", "0.1");
	status = fnPlugin.registerNode(DLInstancer::nodeName, DLInstancer::id, 
		DLInstancer::creator, DLInstancer::initialize, MPxNode::kDependNode);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	return MS::kSuccess;
}


MStatus uninitializePlugin(MObject& obj)
{
	MStatus status;

	MFnPlugin fnPlugin(obj);
	status = fnPlugin.deregisterNode(DLInstancer::id);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	return MS::kSuccess;
}

