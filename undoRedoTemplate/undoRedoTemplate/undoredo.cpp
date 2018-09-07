#include <maya\MFnPlugin.h>
#include "undoredo.h"


MStatus deleteObj::doIt(const MArgList & argList)
{	
	return redoIt();
}

MStatus deleteObj::redoIt()
{
	for (int j = 0; j < 3; j++)
	{
		MString cmd("maya.cmds.polyCube()");
		MGlobal::executePythonCommand(cmd);
		MSelectionList selection;
		MGlobal::getActiveSelectionList(selection);
		MObject selObj;
		selection.getDependNode(j, selObj);
		objectTransforms.append(selObj);
		
	}	
		
	return MS::kSuccess;
}

MStatus deleteObj::undoIt()
{
	
	
	for (unsigned int i = 0; i <objectTransforms.length(); i++)
	{
				
		MGlobal::deleteNode(objectTransforms[i]);
	}

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

