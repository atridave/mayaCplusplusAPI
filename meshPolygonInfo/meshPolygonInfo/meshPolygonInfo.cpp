#include "meshPolygonInfo.h"
#include <maya\MFnPlugin.h>
#include <maya\MItMeshPolygon.h>

MStatus meshPolygonInfo::doIt(const MArgList & argList)
{
	MStatus stat = MStatus::kSuccess;

	MSelectionList selection;
	MGlobal::getActiveSelectionList(selection);

	MDagPath dagPath;
	MObject	 component;

	int i, polyCount, polyIndex, vertCount;
	MPoint p;

	MString txt;
	MItSelectionList iter(selection);

	for (; !iter.isDone(); iter.next())
	{
		iter.getDagPath(dagPath, component);
		MItMeshPolygon polyIter(dagPath, component, &stat);

		if (stat == MS::kSuccess)
		{
			txt += MString("Object:  ") + dagPath.fullPathName() + "\n";
			polyCount = polyIter.count();
			txt += MString("#polygon: ") + polyCount + "\n";

			for (; !polyIter.isDone(); polyIter.next())
			{
				polyIndex = polyIter.index();
				txt += MString("Poly ") + polyIndex + "\n";
				vertCount = polyIter.polygonVertexCount();;
				txt += MString("#vertex : ") + vertCount + "\n";

				for (i = 0; i < vertCount; i++)
				{
					p = polyIter.point(i, MSpace::kWorld);
					txt += MString()+ "("+ p.x+","+p.y+","+p.z+")";
				}
				txt += "\n";
			}
		}
	
	}
		
	MGlobal::displayInfo(txt);
	return MS::kSuccess;
}

void * meshPolygonInfo::creator()
{
	return new meshPolygonInfo;
}




MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "Atri Dave", "1.0", "Any");
	MStatus status = plugin.registerCommand("meshPolygoninfo", meshPolygonInfo::creator);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	MStatus status = plugin.deregisterCommand("meshPolygoninfo");
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}