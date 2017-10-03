#pragma once

#include <maya/MPxCommand.h>
#include <maya/MArgList.h>
#include <maya/MObject.h>
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MDagPath.h>
#include <maya/MItSelectionList.h>
#include <maya/MItMeshVertex.h>






class meshInfo : public MPxCommand
{
public:
	meshInfo() {};
	virtual MStatus doIt(const MArgList& argList);
	static void* creator();
};