#include "meshColorInfo.h"
#include <maya\MFnPlugin.h>
#include <maya\MItMeshFaceVertex.h>

MStatus meshColorInfo::doIt(const MArgList & argList)
{
	MStatus stat = MS::kSuccess;

	MSelectionList selection;
	MGlobal::getActiveSelectionList(selection);

	MDagPath dagPath;
	MObject component;
	MColor c;
	MString txt;
	MItSelectionList iter(selection);

	for (; !iter.isDone(); iter.next())
	{
		iter.getDagPath(dagPath, component);
		MItMeshFaceVertex fvIter(dagPath, component, &stat);

		if (stat == MS::kSuccess)
		{
			txt += MString("Object:") + dagPath.fullPathName() + "\n";
			for (; !fvIter.isDone(); fvIter.next())
			{
				int vertID = fvIter.vertId();
				int faceID = fvIter.faceId();
				int faceVertID = fvIter.faceVertId();

				txt += MString(" FACE ") + faceID + " vertxtID :" + vertID + " faceVertexID " + faceVertID + "\n";

				if (fvIter.hasColor())
				{
					fvIter.getColor(c);
					txt += MString("Color is ") + c.r +", " + c.g + ", " + c.b +", " +c.a + "\n";
				}
				else
				{
					txt += MString("No colors\n");
				}
			}
		}
	}

	MGlobal::displayInfo(txt);
	return MS::kSuccess;
}

void * meshColorInfo::creator()
{
	return new meshColorInfo;
}






MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "Atri Dave", "1.0", "Any");
	MStatus status = plugin.registerCommand("meshColorInfo", meshColorInfo::creator);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	MStatus status = plugin.deregisterCommand("meshColorInfo");
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}