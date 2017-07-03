#include <maya\MFnPlugin.h>
#include "ShapeSnape.h"



MStatus ShapeSnape::doIt(const MArgList&)
{
	MStatus status = MStatus::kSuccess;

	//Selection List find it if there are only teo mesh slected and thet are polygon only
	
	MSelectionList selectionList;
	MGlobal::getActiveSelectionList(selectionList);

	if (selectionList.length() != 2)
	{
		MGlobal::displayError("Please select two polygon object");
		return MStatus::kFailure;
	}

	//Geather information for object and its vertexes 

	MDagPath modifidedPath;
	MObject modifiedComponent;

	selectionList.getDagPath(0, modifidedPath, modifiedComponent);
	selectionList.getDagPath(1, originalPath);

	MFnMesh modifiedFn(modifidedPath);
	MFnMesh originFn(originalPath);
	

	//check if componad is slected !!
	//need to check on c++ side what is !variable meaning 

	if (!modifiedComponent.isNull())
	{
		MFnSingleIndexedComponent modifiedComponentFn(modifiedComponent);
		modifiedComponentFn.getElements(pVertaxIDs);
	}
	else
	{
		int num = originFn.numVertices();
		for (int i = 0; i < num; i++)
		{
			pVertaxIDs.append(i);
		}
		
	}

	
	for (unsigned int i = 0; i < pVertaxIDs.length(); i++)
	{
		MPoint modPt;
		status = modifiedFn.getPoint(pVertaxIDs[i], modPt, MSpace::kObject);

		if (!status)
		{
			MGlobal::displayError("error getting modified objects point postion!");
			return MStatus::kFailure;
		}
		m_pointArray.append(modPt);

		MPoint origPt;
		status = originFn.getPoint(pVertaxIDs[i], origPt, MSpace::kObject);


		if (!status)
		{
			MGlobal::displayError("error getting original objects point postion!");
			return MStatus::kFailure;
		}
		m_undoPoints.append(origPt);

	}

	return redoIt();


	MGlobal::displayInfo("Hello World!");	
	MGlobal::displayInfo("I am working from ShapeH");	
	
}

MStatus ShapeSnape::undoIt()
{
	MStatus status;
	MFnMesh originalFn(originalPath);


	for (unsigned int i = 0; i < pVertaxIDs.length(); i++)
	{
		MPoint modPt;
		status = originalFn.setPoint(pVertaxIDs[i], m_undoPoints[i], MSpace::kObject);

		if (!status)
		{
			MGlobal::displayError("error getting modified objects point postion!");
			return status;
		}

	}



	return MStatus::kSuccess;
}

MStatus ShapeSnape::redoIt()
{
	MStatus status;
	MFnMesh originalFn(originalPath);


	for (unsigned int i = 0; i < pVertaxIDs.length(); i++)
	{
		MPoint modPt;
		status = originalFn.setPoint(pVertaxIDs[i], m_pointArray[i], MSpace::kObject);

		if (!status)
		{
			MGlobal::displayError("error getting modified objects point postion!");
			return status;
		}		

	}

	return MStatus::kSuccess;
}

bool ShapeSnape::isUndoable() const
{
	return true;
}



void* ShapeSnape::creator() 
{ 
	return new ShapeSnape; 
}

MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "Atri Dave", "1.0", "Any");
	MStatus status = plugin.registerCommand("shapeSnape", ShapeSnape::creator);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	MStatus status = plugin.deregisterCommand("shapeSnape");
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}