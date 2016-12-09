#pragma once
#include <maya/MPxCommand.h>
#include <maya/MDagPath.h>
#include <maya/MDGModifier.h>
#include <maya/MDagModifier.h>
#include <maya/MSelectionList.h>
#include <maya/MSyntax.h>
#include <maya/MString.h>
#include <maya/MArgDatabase.h>
#include <maya/MGlobal.h>
#include <maya/MItDependencyNodes.h>

#include "DLInstancer.h"

class DLCreateInstancerCmd : public MPxCommand
{
public:
	DLCreateInstancerCmd();
	virtual ~DLCreateInstancerCmd();
	static void* creator();

	static MSyntax initializeSyntax();
	
	virtual MStatus doIt(const MArgList& args);
	virtual MStatus redoIt();
	virtual MStatus undoIt();
	virtual bool isUndoable() const;

	static const MString cmdName;

private:
	// MEMBERS //
	MString name_;
	MDGModifier dgMod_;
	MDagModifier dagMod_;
	MObject instanceMesh_;
	MObject referenceMesh_;
	bool createBaseMeshes_;
	MSelectionList selectionList_;
	enum dlObjectType {kCube, kPlane, kNull};

	// METHODS //
	MStatus dlParseArgs_(const MArgList& args);
	MObject dlCreateObject_(dlObjectType type, MString& name2);
	MPlug dlGetDefaultShaderPlug_();

};