#include "createRod.h"
#include <maya\MFnPlugin.h>



MStatus createRod::doIt(const MArgList & argList)
{

	//MObject create(
	//	int 	numVertices,
	//	int 	numPolygons,
	//	const MFloatPointArray & 	vertexArray,
	//	const MIntArray & 	polygonCounts,
	//	const MIntArray & 	polygonConnects,
	//	MObject 	parentOrOwner = MObject::kNullObj,
	//	MStatus * 	ReturnStatus = NULL
	//)

	MPoint p0(0, 0, 10);
	MPoint p1(-15, 15, -20);
	double radius = 10.0;
	int segments = 20;
	MObject myMesh;
	MFnMesh meshfn;
	MStatus stat;
	MPointArray vertexArray;
	MIntArray polyCount;
	int numPolygones;
	MIntArray polyConnects;
	//MString txt = "";



	makeRodData(p0,p1,radius,segments);


	////txt += vertexArray.length();


	////MGlobal::displayInfo(txt);


	//myMesh = meshfn.create(vertexArray.length(), numPolygones, vertexArray, polyCount, polyConnects, MObject::kNullObj, &stat);

	//if (!stat)
	//	stat.perror("Unable to create Mesh");

	//meshfn.updateSurface();


	//MString cmd("maya.cmds.sets('" + meshfn.name() + "', e = 1, fe = 'initialShadingGroup')");
	//MGlobal::executePythonCommand(cmd);

	return MS::kSuccess;
}

void * createRod::creator()
{
	return new createRod;
}


MStatus makeRodData(const MPoint &p0, const MPoint &p1, const double radius, const int segments)
{
	
	MVector vec(p1 - p0);
	MVector up(0.0, 1.0, 0.0);

	MVector xAxis, yAxis, zAxis;

	yAxis = vec.normal();
	if (up.isParallel(yAxis, 0.1))
	{
		up = MVector(1.0, 0.0, 0.0);
	}
	xAxis = yAxis ^ up;
	zAxis = (xAxis ^ yAxis).normal();
	xAxis = (yAxis ^ zAxis).normal();

	MPoint p,p2;
	//verts.append(p.origin);
	double x, z;



	for (int i = 0; i <segments + 1; i++)
	{
		double angle = mapRange(i, 0, segments, 0, 2 * M_PI);

		x = radius*cos(angle);
		z = radius*sin(angle);
		
		p = p0 + x*xAxis + z*zAxis;
		p2 = p1 + x*xAxis + z*zAxis;

		//verts.append(p);

		//polyCount[i] = 3;
		//polyConnects.append(0);
		//polyConnects.append(i + 1);
		//polyConnects.append(i);

		MString cmd = ("maya.cmds.spaceLocator(p=(");
		cmd += p2.x;
		cmd += ",";
		cmd += p2.y;
		cmd += ",";
		cmd += p2.z;
		cmd += "))";
		MGlobal::executePythonCommand(cmd);



	}


	return MStatus::kSuccess;
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


