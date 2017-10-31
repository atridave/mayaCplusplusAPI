#include "createCube.h"
#include <maya\MFnPlugin.h>






MStatus createCube::doIt(const MArgList & argList)
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

	MPointArray vertxes;
	vertxes.clear();
	vertxes.setLength(4);
	vertxes.set(0, 0, -1, 1, 1);
	vertxes.set(1, 0, -1, -1, 1);
	vertxes.set(2, 0, 1, -1, 1);
	vertxes.set(3, 0, 1, 1, 1);

	MIntArray polyCount;
	polyCount.clear();
	polyCount.insert(4, 0);

	MIntArray polyConnects;
	polyConnects.clear();
	polyConnects.setLength(4);
	polyConnects.set(0, 0);
	polyConnects.set(1, 1);
	polyConnects.set(2, 2);
	polyConnects.set(3, 3);


	MString txt = "Hello Till this I am working fine \n";
	
	

	txt += MString("vertexes are :") + "x:" +vertxes[3].x +" y:"+ vertxes[3].y +" z:"+ vertxes[3].z + "\n";
	txt += MString("polyCount:") + polyCount[0];
	MGlobal::displayInfo(txt);




	myMesh = meshfn.create(numVertices, numPolygones, vertxes,polyCount, polyConnects,MObject::kNullObj,&stat);

	if (!stat)
		stat.perror("Unable to create Mesh");
	
	meshfn.updateSurface();
	
	
	MString cmd("maya.cmds.sets('"+ meshfn.name()+"', e = 1, fe = 'initialShadingGroup')");
	MGlobal::executePythonCommand(cmd);

	return MS::kSuccess;
}

void * createCube::creator()
{
	return new createCube;
}




MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "Atri Dave", "1.0", "Any");
	MStatus status = plugin.registerCommand("createCube", createCube::creator);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	MStatus status = plugin.deregisterCommand("createCube");
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}

