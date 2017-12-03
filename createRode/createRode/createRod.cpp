#include "createRod.h"
#include "maya/MFnPlugin.h"



//
//MObject create(int 	numVertices,
//	int 	numPolygons,
//	const MFloatPointArray & 	vertexArray,
//	const MIntArray & 	polygonCounts,
//	const MIntArray & 	polygonConnects,
//	MObject 	parentOrOwner = MObject::kNullObj,
//	MStatus * 	ReturnStatus = NULL
//)

MStatus createRod::doIt(const MArgList & argList)
{
	MPoint p0(0,0,10);
	MPoint p1(0,0,-10);
	double radius = 10;
	int segments = 20;
	MPointArray verts;
	int numPolygones;
	MIntArray polyCount;
	MIntArray polyConnects;
	MString txt = "";

	MVector vec(p0-p1);
	MVector up(0, 1, 0);
	MVector xAxis, yAxis, zAxis;

	double length = vec.length();

	yAxis = vec.normal();

	if (up.isParallel(yAxis, 0.1))
	{
		up = MVector(1, 0, 0);
	}

	xAxis = yAxis ^ up; 

	

	txt += up.x;
	txt += up.y;
	txt += up.z;
	


	//MString cmd = ("maya.cmds.spaceLocator(p=(");

	//cmd += p0.x;
	//cmd += ",";
	//cmd += p0.y;
	//cmd += ",";
	//cmd += p0.z;
	//cmd += "))";
	//MGlobal::executePythonCommand(cmd);

	MGlobal::displayInfo(txt);





	

	return MS::kSuccess;
}

void * createRod::creator()
{
	return new createRod;
}











MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "Atri Dave", "1.0", "Any");
	MStatus status = plugin.registerCommand("createRod", createRod::creator);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	MStatus status = plugin.deregisterCommand("createRod");
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}



MStatus createRodData(const MPoint & p0, const MPoint & p1, const double radius, const unsigned int segment, MPointArray & verts, int & numPolygones, MIntArray & polyCount, MIntArray & polyConnects)
{


	return MS::kSuccess;
}


