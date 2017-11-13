#include "makeCircle.h"
#include <maya\MFnPlugin.h>



MStatus makeCircle::doIt(const MArgList & argList)
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



	makeCircleData(radius, segments, vertexArray, numPolygones, polyCount, polyConnects);


	//myMesh = meshfn.create(vertexArray.length(), numPolygones, vertexArray, polyCount, polyConnects, MObject::kNullObj, &stat);

	//if (!stat)
	//	stat.perror("Unable to create Mesh");

	//meshfn.updateSurface();


	//MString cmd("maya.cmds.sets('" + meshfn.name() + "', e = 1, fe = 'initialShadingGroup')");
	//MGlobal::executePythonCommand(cmd);

	return MS::kSuccess;
}

void * makeCircle::creator()
{
	return new makeCircle;
}


MStatus makeCircleData(const double radius, const int segments, MPointArray &verts, int &numPolygones, MIntArray &polyCount, MIntArray &polyConnects)
{
	verts.clear();
	polyCount.clear();
	polyConnects.clear();
	MPoint p;

	for (int j = 0; j <segments + 1; j++)
		{	
			double angle = mapRange(j, 0, segments, 0, 2 * M_PI);

			p.x = radius*cos(angle);
			p.z = radius*sin(angle);
			p.y = 0.0f;

			verts.append(p);
			MString cmd = ("maya.cmds.spaceLocator(p=(");
			cmd += p.x;
			cmd += ",";
			cmd += p.y;
			cmd += ",";
			cmd += p.z;
			cmd += "))";
			MGlobal::executePythonCommand(cmd);
		}
	

	numPolygones = segments*segments;

	polyCount.setLength(numPolygones);
	for (int i = 0; i < numPolygones; i++)
	{
		polyCount[i] = 4;
	}



	for (int i = 0; i < segments; i++)
	{
		for (int j = 0; j < segments; j++)
		{
			polyConnects.append(linerIndex(i, j, segments + 1, segments + 1));
			polyConnects.append(linerIndex(i, j + 1, segments + 1, segments + 1));
			polyConnects.append(linerIndex(i + 1, j + 1, segments + 1, segments + 1));
			polyConnects.append(linerIndex(i + 1, j, segments + 1, segments + 1));
		}

	}
	return MStatus::kSuccess;
}



MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "Atri Dave", "1.0", "Any");
	MStatus status = plugin.registerCommand("makeCircle", makeCircle::creator);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	MStatus status = plugin.deregisterCommand("makeCircle");
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}


