#pragma once

#include <maya/MPxCommand.h>
#include <maya/MDGModifier.h>
#include <maya/MSyntax.h>
#include <maya/MObject.h>
#include <maya/MArgDatabase.h>
#include <maya/MArgList.h>
#include <maya/MSelectionList.h>
#include <maya/MPlugArray.h>
#include <maya/MPlug.h>

#include "DLCommon.h"
#include "DLInstancer.h"

class DLUpdateMaterialsCmd : public MPxCommand
{
public:
	DLUpdateMaterialsCmd();
	virtual ~DLUpdateMaterialsCmd();
	static void* creator();

	static MSyntax initializeSyntax();

	virtual MStatus doIt(const MArgList& args);
	virtual MStatus redoIt();
	virtual MStatus undoIt();
	virtual bool isUndoable() const;

	static const MString cmdName;

private:
	// MEMBERS //
	MDGModifier dgMod_;
	MSelectionList selectionList_;
	MObject selectedNode_;
	MObject instancerNode_;

	// METHODS //
	MStatus dlParseArgs_(const MArgList& args);
	MStatus dlGetInstancerNode_();
	MStatus dlConnectMaterials_();

};