#include <maya/MFnPlugin.h>

MStatus initializePlugin(MObject& obj)
{
	MStatus status;

	MFnPlugin fnPlugin(obj, "DeeLan", "0.1");

	return MS::kSuccess;
}