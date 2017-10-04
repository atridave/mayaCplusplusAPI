#include "meshEageInfo.h"
#include <maya\MFnPlugin.h>
#include <maya\MItMeshEdge.h>





MStatus meshEageInfo::doIt(const MArgList & argList)
{
	MStatus stat = MS::kSuccess;
	MSelectionList selection;
	MGlobal::getActiveSelectionList(selection);

	MDagPath dagPath;
	MObject component;
	int eageCount, v0Index, v1Index, eageIndex;
	MPoint v0, v1;

	MString txt;

	MItSelectionList iter(selection);

	for (; !iter.isDone(); iter.next())
	{
		iter.getDagPath(dagPath, component);
		MItMeshEdge eageIter(dagPath, component,&stat);
		if (stat == MS::kSuccess)
		{
			txt += dagPath.fullPathName() + "\n";
			eageCount = eageIter.count();
			txt += MString("#Eages: ") + eageCount + "\n";

			for (; !eageIter.isDone(); eageIter.next())
			{
				eageIndex = eageIter.index();
				v0Index = eageIter.index(0);
				v1Index = eageIter.index(1);

				v0 = eageIter.point(0, MSpace::kWorld);
				v1 = eageIter.point(1, MSpace::kWorld);

				txt = txt + "Eage " + eageIndex + ": " +
					v0Index + "(" +
					v0.x + ", " + v0.y + ", " + v0.z + ")" +
					v1Index + "(" +
					v1.x + ", " + v1.y + ", " + v1.z + ")";
			}

		}

		
	}


	MGlobal::displayInfo(txt);
	return MS::kSuccess;
}




void * meshEageInfo::creator()
{
	return new meshEageInfo;
}




MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "Atri Dave", "1.0", "Any");
	MStatus status = plugin.registerCommand("meshEageInfo", meshEageInfo::creator);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	MStatus status = plugin.deregisterCommand("meshEageInfo");
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}