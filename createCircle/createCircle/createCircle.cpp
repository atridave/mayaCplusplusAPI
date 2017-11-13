#include <maya\MFnPlugin.h>
#include "createCircle.h"



MStatus createCircle::doIt(const MArgList & argList)
{

	double radius = 10.0;
	int segments = 20;
	MObject myMesh;
	MFnMesh meshfn;
	MStatus stat;
	MPointArray vertexArray;
	MIntArray polyCount;
	int numPolygones;
	MIntArray polyConnects;


	createCircleData(radius, segments, vertexArray, numPolygones, polyCount, polyConnects);


	myMesh = meshfn.create(vertexArray.length(), numPolygones, vertexArray, polyCount, polyConnects, MObject::kNullObj, &stat);

	if (!stat)
		stat.perror("Unable to create Mesh");

	meshfn.updateSurface();


	MString cmd("maya.cmds.sets('" + meshfn.name() + "', e = 1, fe = 'initialShadingGroup')");
	MGlobal::executePythonCommand(cmd);

	return MS::kSuccess;
}



void * createCircle::creator()
{
	return new createCircle;
}


MStatus createCircleData(const double radius, const int segments, MPointArray &verts, int &numPolygones, MIntArray &polyCount, MIntArray &polyConnects)
{
	verts.clear();
	polyCount.clear();
	polyConnects.clear();

	numPolygones = segments+1;
	polyCount.setLength(segments + 1);

	MPoint p;
	verts.append(p.origin);
	



	for (int i = 0; i <segments+1; i++)
	{
		double angle = mapRange(i, 0, segments, 0, 2 * M_PI);

		p.x = radius*cos(angle);
		p.z = radius*sin(angle);
		p.y = 0.0f;

		verts.append(p);

		polyCount[i] = 3;
		polyConnects.append(0);
		polyConnects.append(i + 1);
		polyConnects.append(i);
		
	}


	return MStatus::kSuccess;
}



MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "Atri Dave", "1.0", "Any");
	MStatus status = plugin.registerCommand("createCircle", createCircle::creator);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	MStatus status = plugin.deregisterCommand("createCircle");
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}


