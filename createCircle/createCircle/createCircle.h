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






class createCircle : public MPxCommand
{
public:
	createCircle() {};
	virtual MStatus doIt(const MArgList& argList);
	static void* creator();
};

MStatus createCircleData(const double radius, const int segments, MPointArray &verts, int &numPolygones, MIntArray &polyCount, MIntArray &polyConnects);

double mapRange(double value, double minX, double maxX, double minY, double maxY)
{
	return (minY + ((double((value - minX) / (maxX - minX)))*(maxY - minY)));
}

