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






class createSphere : public MPxCommand
{
public:
	createSphere() {};
	virtual MStatus doIt(const MArgList& argList);
	static void* creator();
};

MStatus makeSphereData(const double radius, const int segments, MPointArray &verts, int &numPolygones, MIntArray &polyCount);

double mapRange(double value, double minX, double maxX, double minY, double maxY)
{

	//double xDiff = (maxX - minX);
	//double yDiff = (maxY - minY);
	//double unitVal = (double((value - minX) / xDiff));
	//double newValue = minY + (unitVal*yDiff);
	return (minY + ((double((value - minX) / (maxX - minX)))*(maxY - minY)));
}