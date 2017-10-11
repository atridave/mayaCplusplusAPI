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






class molecule1 : public MPxCommand
{
public:
	molecule1() {};
	virtual MStatus doIt(const MArgList& argList);
	static void* creator();
};

int linerIndex(const int r, const int c, const int nRows, const int nCols)
{ 
	return ((r%nRows)*nCols) + (c%nCols);
}

MStatus genBall(const MPoint &center, const double radius, const unsigned int nSegs, int &nPolys, MPointArray &verts, MIntArray &polyCount, MIntArray &polyConnects);

MStatus genRod(const MPoint &p0, const MPoint &p1, const double radius, const unsigned int nSegs, int &nPolys, MPointArray &verts, MIntArray &polyCount, MIntArray &polyConnects);

