#pragma once

#include <maya\MPxCommand.h>
#include <maya\MArgList.h>
#include <maya\MGlobal.h>
#include <maya\MObject.h>
#include <maya\MSyntax.h>
#include <maya\MPoint.h>
#include <maya\MArgDatabase.h>
#include <maya\MSelectionList.h>
#include <maya\MItSelectionList.h>
#include <maya\MDoubleArray.h>
#include <maya\MDagPath.h>
#include <maya\MFnMesh.h>


class ClosestPointOnMesh : public MPxCommand
{
public:
	ClosestPointOnMesh() {};
	virtual MStatus doIt(const MArgList &args);
	static void* creator();
	static MSyntax newSyntax();

};