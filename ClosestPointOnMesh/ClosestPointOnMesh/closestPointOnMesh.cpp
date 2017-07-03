#include "closestPointOnMesh.h"
#include <maya\MFnPlugin.h>


const char *pointXFlage = "-px";
const char *pointYFlage = "-py";
const char *pointZFlage = "-pz";
const char *pointXLongFlage = "-pointX";
const char *pointYLongFlage = "-pointY";
const char *pointZLongFlage = "-pointZ";

MStatus ClosestPointOnMesh::doIt(const MArgList &args)
{
	MGlobal::displayInfo("I have started woirking fine");
	MGlobal::displayInfo("I am updated too");
	MStatus status = MStatus::kSuccess;

	//intialize point to origin
	MPoint point(MPoint::origin);

	// sysntax Parser
	MArgDatabase argData(syntax(), args, &status);

	if (argData.isFlagSet(pointXFlage))
	{
		argData.getFlagArgument(pointXFlage, 0, point.x);
	}
	if (argData.isFlagSet(pointYFlage))
	{
		argData.getFlagArgument(pointYFlage, 0, point.y);
	}

	if (argData.isFlagSet(pointZFlage))
	{
		argData.getFlagArgument(pointZFlage, 0, point.z);
	}

	MSelectionList selectionList;
	status = MGlobal::getActiveSelectionList(selectionList);

	if (!status)
	{
		MGlobal::displayError("Error Getting active selection!");
		return MStatus::kFailure;
	}

	MItSelectionList iterList(selectionList, MFn::kMesh, &status);

	if (!status)
	{
		MGlobal::displayError("Error for intializing selection iterator!");
		return MStatus::kFailure;
	}

	MDoubleArray returnPoints;

	for (; !iterList.isDone(); iterList.next())
	{
		MDagPath geoPath;
		iterList.getDagPath(geoPath);

		MPoint closestPoint;
		int polygonID;

		MFnMesh meshFn(geoPath);

		//pass the point form argument and find closest point on the mesh 
		// it is on world space
		// send faceID too 

		meshFn.getClosestPoint(point, closestPoint, MSpace::kWorld, &polygonID);

		// Out put 

		MString outPutStr = "\n --------------------Closest Vertax --------------\n\n";
		outPutStr += "\tMesh full path:" + geoPath.fullPathName() + "\n";
		outPutStr += MString("\tFace ID: ") + polygonID + "\n";
		outPutStr += "\tClosest Vertex Postion : \n";
		outPutStr += MString("\t\tX: ") + closestPoint.x + "\n";
		outPutStr += MString("\t\tY: ") + closestPoint.y + "\n";
		outPutStr += MString("\t\tZ: ") + closestPoint.z + "\n";
		MGlobal::displayInfo(outPutStr);

		// append closest points to variables 

		returnPoints.append(closestPoint.x);
		returnPoints.append(closestPoint.y);
		returnPoints.append(closestPoint.z);

	}

	setResult(returnPoints);

	return MS::kSuccess;
}

void * ClosestPointOnMesh::creator()
{
	return new ClosestPointOnMesh;
}

MSyntax ClosestPointOnMesh::newSyntax()
{
	MSyntax syntax;

	syntax.addFlag(pointXFlage, pointXLongFlage, MSyntax::kDouble);
	syntax.addFlag(pointYFlage, pointYLongFlage, MSyntax::kDouble);
	syntax.addFlag(pointZFlage, pointZLongFlage, MSyntax::kDouble);

	return syntax;
}





MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "Atri Dave", "1.0", "Any");
	MStatus status = plugin.registerCommand("closestPointOnMesh", ClosestPointOnMesh::creator,ClosestPointOnMesh::newSyntax);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;

}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	MStatus status = plugin.deregisterCommand("closestPointOnMesh");
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}