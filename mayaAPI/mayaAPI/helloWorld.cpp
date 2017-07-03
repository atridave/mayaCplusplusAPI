#include <maya/MFnPlugin.h>
#include <iostream>

#include "helloWorld.h";

using namespace std;


MStatus HelloWorld::doIt(const MArgList&)
{
	MGlobal::displayInfo("Hello World!");
	MGlobal::displayWarning("Time to go to sleep Atri !!!!!!!!!!");
	MString sphereName;
	MGlobal::executeCommand("polySphere - n\"mySphere\";", sphereName);
	cout << "sphere name is :: " << sphereName.asChar() << endl;
	cout << "hello World";
	MGlobal::executePythonCommand("maya.cmds.polyCube()");
	MGlobal::displayInfo(sphereName.asChar());
	return MS::kSuccess;
}



void* HelloWorld::creator() { return new HelloWorld; }

MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "Atri Dave", "1.0", "Any");
	MStatus status = plugin.registerCommand("helloWorld", HelloWorld::creator);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	MStatus status = plugin.deregisterCommand("helloWorld");
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}