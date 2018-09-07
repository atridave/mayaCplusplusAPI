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
#include <maya/MDistance.h>
#include <maya/MDagPathArray.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MFloatArray.h>
#include <maya/MDagModifier.h>





class molecule2Cmd : public MPxCommand
{
public:
	virtual MStatus doIt(const MArgList&);
	virtual MStatus redoIt();
	virtual MStatus undoIt();
	virtual bool isUndoable() const { return true; }
	static void* creator();
	static MSyntax newSyntax();
	MObjectArray objectTransforms;


private:
	MDistance radius;
	int segs;
	double ballRodRatio;
	MDagPathArray selMeshes;

	

};

int linerIndex(const int r, const int c, const int nRows, const int nCols)
{
	return ((r%nRows)*nCols) + (c%nCols);
}

MStatus genBall(const MPoint &center, const double radius, const unsigned int nSegs, int &nPolys, MPointArray &verts, MIntArray &polyCount, MIntArray &polyConnects,const bool genUVs,MFloatArray &uCoords,MFloatArray &vCoords,MIntArray &fvUVIDs);

MStatus genRod(const MPoint &p0, const MPoint &p1, const double radius, const unsigned int nSegs, int &nPolys, MPointArray &verts, MIntArray &polyCount, MIntArray &polyConnects, const bool genUVs, MFloatArray &uCoords, MFloatArray &vCoords, MIntArray &fvUVIDS);

