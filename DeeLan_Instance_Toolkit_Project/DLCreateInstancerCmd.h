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
	bool autoUVUpdates_;
	bool autoMaterialUpdates_;
	bool createBaseMeshes_;
	MSelectionList selectionList_;

	// METHODS //
	MStatus dlParseArgs_(const MArgList& args);
	MObject dlCreateCube_();
	MObject dlCreatePlane_();
	bool dlIsShapeNode(const MDagPath& path);
	MStatus dlGetShapeNode_(MDagPath& path, bool intermediate = false);

};