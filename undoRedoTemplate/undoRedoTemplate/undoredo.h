#pragma once

#include <maya/MPxCommand.h>
#include <maya/MArgList.h>
#include <maya/MObject.h>
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MDagPath.h>
#include <maya/MFnMesh.h>
#include <maya/MPointArray.h>
#include <maya/MIntArray.h>
#include <maya/MItSelectionList.h>
#include <maya/MItMeshEdge.h>
#include <maya/MDagModifier.h>



class deleteObj : public MPxCommand
{
public:
	deleteObj() {};
	virtual MStatus doIt(const MArgList& argList);
	static void* creator();
	virtual MStatus redoIt();
	virtual MStatus undoIt();
	MObjectArray objectTransforms;
	virtual bool isUndoable() const { return true; }
};

