#pragma once

#include <maya\MArgList.h>
#include <maya\MObject.h>
#include <maya\MGlobal.h>
#include <maya\MPxCommand.h>
#include <maya\MSelectionList.h>
#include <maya\MDagPath.h>
#include <maya\MFnMesh.h>
#include <maya\MFnSingleIndexedComponent.h>
#include <maya\MIntArray.h>
#include <maya\MPointArray.h>


class ShapeSnape : public MPxCommand
{
public:
	ShapeSnape() {};
	virtual ~ShapeSnape() {	};

	static void* creator();
	
	MStatus doIt(const MArgList& argList);
	MStatus undoIt();
	MStatus redoIt();
	bool 	isUndoable() const;
	MDagPath originalPath;
	MObject originalComponent;
	MIntArray pVertaxIDs;
	MPointArray m_pointArray;
	MPointArray m_undoPoints;
	
	
	
	
};
