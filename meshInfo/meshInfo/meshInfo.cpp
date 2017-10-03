#include "meshInfo.h"
#include <maya\MFnPlugin.h>
#include <maya\MFnMesh.h>



MStatus meshInfo::doIt(const MArgList & argList)
{
	MStatus status = MS::kSuccess;
	MSelectionList selection;
	MGlobal::getActiveSelectionList(selection);

	MDagPath dagPath;
	MObject component;
	MString txt;

	for (int i = 0; i < selection.length(); i++)
	{
		selection.getDagPath(i, dagPath, component);
		
		txt += dagPath.fullPathName() + "\n";

		MFnMesh polyMesh(dagPath);
		int num = polyMesh.numVertices();		
		txt += MString(dagPath.fullPathName())+ "has " +("Vertexis :") + num + "\n";

		for (int j = 0; j < num; j++)
		{
			MPoint vPos;
			polyMesh.getPoint(j, vPos, MSpace::kWorld);
			txt += MString("VertexID:") + j + "----->" + "  X is: " + vPos.x + " Y is: " + vPos.y + " Z is: " + vPos.z + "\n";
			
		}

		
	}
	MGlobal::displayInfo(txt);
	return MS::kSuccess;

}






void * meshInfo::creator()
{
	return new meshInfo;
}




MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "Atri Dave", "1.0", "Any");
	MStatus status = plugin.registerCommand("meshInfo", meshInfo::creator);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	MStatus status = plugin.deregisterCommand("meshInfo");
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}