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


	
	makeSphereData(radius, segments, vertexArray, numPolygones, polyCount, polyConnects);
	
	
	//txt += vertexArray.length();
	

	//MGlobal::displayInfo(txt);


	myMesh = meshfn.create(vertexArray.length(), numPolygones, vertexArray, polyCount, polyConnects, MObject::kNullObj, &stat);

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


MStatus makeSphereData(const double radius, const int segments, MPointArray &verts,int &numPolygones, MIntArray &polyCount,MIntArray &polyConnects)
{
	verts.clear();
	polyCount.clear();
	polyConnects.clear();
	MPoint p;	

	for (int i = 0; i < (segments+1); i++)
	{
		double lat = mapRange(i, 0, segments, 0, M_PI);		
		for (int j = 0; j <segments+1; j++)
		{
			double lon = mapRange(j, 0, segments, 0, 2*M_PI);

			 p.x = radius*sin(lat)*cos(lon);
			 p.z = radius*sin(lat)*sin(lon);
			 p.y = radius*cos(lat);

			 verts.append(p);
			 //MString cmd = ("maya.cmds.spaceLocator(p=(");
			 //cmd += p.x;
			 //cmd += ",";
			 //cmd += p.y;
			 //cmd += ",";
			 //cmd += p.z;
			 //cmd += "))";
			 //MGlobal::executePythonCommand(cmd);
		}
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
			polyConnects.append(linerIndex(i, j, segments + 1, segments+1));
			polyConnects.append(linerIndex(i, j + 1, segments + 1, segments+1));
			polyConnects.append(linerIndex(i + 1, j + 1, segments + 1, segments+1));
			polyConnects.append(linerIndex(i + 1, j, segments + 1, segments+1));						
		}

	}
	return MStatus::kSuccess;
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


