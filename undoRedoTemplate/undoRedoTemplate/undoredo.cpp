#include <maya\MFnPlugin.h>
#include "undoredo.h"



MStatus deleteObj::doIt(const MArgList & argList)
{

	MGlobal::displayInfo("I am in doit ");

	return redoIt();
}

MStatus deleteObj::redoIt()
{
	MSelectionList selection;
	MGlobal::getActiveSelectionList(selection);

	MObject selObj;

	selection.getDependNode(0, selObj);


	MGlobal::deleteNode(selObj);
	MGlobal::displayInfo("I have deleted the mesh"); 

	return MS::kSuccess;
}

MStatus deleteObj::undoIt()
{
	MGlobal::displayInfo("I am in undoing it ");
	return MS::kSuccess;
}




void * deleteObj::creator()
{
	return new deleteObj;
}







MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "Atri Dave", "1.0", "Any");
	MStatus status = plugin.registerCommand("deleteObj", deleteObj::creator);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	MStatus status = plugin.deregisterCommand("deleteObj");
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}


