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



//MObject create(int 	numVertices,
//	int 	numPolygons,
//	const MFloatPointArray & 	vertexArray,
//	const MIntArray & 	polygonCounts,
//	const MIntArray & 	polygonConnects,
//	MObject 	parentOrOwner = MObject::kNullObj,
//	MStatus * 	ReturnStatus = NULL
//)



class createRod : public MPxCommand
{
public:
	createRod() {};
	virtual MStatus doIt(const MArgList& argList);
	static void* creator();
};


//MStatus createRodData(const MPoint &p0, const MPoint &p1,const double radius,const unsigned int segment, MPointArray &verts, int &numPolygones, MIntArray &polyCount, MIntArray &polyConnects);
MStatus createRodData(const MPoint &p0);


double mapRange(double value, double minX, double maxX, double minY, double maxY)
{
	return (minY + ((double((value - minX) / (maxX - minX)))*(maxY - minY)));
}