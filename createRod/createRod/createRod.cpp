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
	MPoint p1(-10, 30, -25);
	double radius = 10.0;
	int segments = 20;
	MObject myMesh;
	MFnMesh meshfn;
	MStatus stat;
	MPointArray vertexArray;
	MIntArray polyCount;
	int numPolygones;
	MIntArray polyConnects;
	MString txt = "I am finished the roddata";



	makeRodData(p0,p1,radius,segments, vertexArray,numPolygones,polyCount, polyConnects);

	
	txt += vertexArray.length();

	MGlobal::displayInfo(txt);


	myMesh = meshfn.create(vertexArray.length(), numPolygones, vertexArray, polyCount, polyConnects, MObject::kNullObj, &stat);

	MGlobal::displayInfo(txt);

	if (!stat)
		stat.perror("Unable to create Mesh");

	meshfn.updateSurface();


	MString cmd("maya.cmds.sets('" + meshfn.name() + "', e = 1, fe = 'initialShadingGroup')");
	MGlobal::executePythonCommand(cmd);

	return MS::kSuccess;
}

void * createRod::creator()
{
	return new createRod;
}


MStatus makeRodData(const MPoint &p0, const MPoint &p1, const double radius, const int segments, MPointArray &vertexArray, int &numPolygones, MIntArray &polyCount, MIntArray &polyConnects)
{
	
	vertexArray.clear();
	polyConnects.clear();


	numPolygones = segments;
	polyCount.setLength(segments);


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

	MPoint p;
	double x, z;

	
	int j = 0;
	for (int i = 0; i <segments + 1; i++)
	{
		double angle = mapRange(i, 0, segments, 0, 2 * M_PI);

		x = radius*cos(angle);
		z = radius*sin(angle);
		
		p = p0 + x*xAxis + z*zAxis;	

		vertexArray.append(p);

		p = p1 + x*xAxis + z*zAxis;
		vertexArray.append(p);

		int test = vertexArray.length();
		

		polyCount[i] = 4;

		
		//polyConnects.append(i);
		//polyConnects.append(i + 1);
		//polyConnects.append(i + 3);
		//polyConnects.append(i + 2);

	}

	for (int i = 0; i < (segments)*2; i++)
	{
		polyConnects.append(i);
		polyConnects.append(i + 1);
		polyConnects.append(i + 3);
		polyConnects.append(i + 2);
		i += 1;
	}




	//for (int i = 0; i < (segments+1)*2; i++)
	//{
	//	MString cmd = ("maya.cmds.spaceLocator(p=(");
	//	cmd += vertexArray[i][0];
	//	cmd += ",";
	//	cmd += vertexArray[i][1];
	//	cmd += ",";
	//	cmd += vertexArray[i][2];
	//	cmd += "))";
	//	MGlobal::executePythonCommand(cmd);

	//}


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


