#include "createSphere.h"
#include <maya\MFnPlugin.h>






MStatus createSphere::doIt(const MArgList & argList)
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

	MObject myMesh;
	MFnMesh meshfn;
	int numVertices = 4;
	int numPolygones = 1;
	MStatus stat;

	




	myMesh = meshfn.create(numVertices, numPolygones, vertxes, polyCount, polyConnects, MObject::kNullObj, &stat);

	if (!stat)
		stat.perror("Unable to create Mesh");

	meshfn.updateSurface();


	MString cmd("maya.cmds.sets('" + meshfn.name() + "', e = 1, fe = 'initialShadingGroup')");
	MGlobal::executePythonCommand(cmd);

	return MS::kSuccess;
}

void * createSphere::creator()
{
	return new createSphere;
}




MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "Atri Dave", "1.0", "Any");
	MStatus status = plugin.registerCommand("createSphere", createSphere::creator);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	MStatus status = plugin.deregisterCommand("createSphere");
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}

