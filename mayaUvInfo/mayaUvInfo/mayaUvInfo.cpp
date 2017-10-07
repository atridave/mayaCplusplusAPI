#include "mayaUvInfo.h"
#include <maya\MFnPlugin.h>
#include <maya\MFnMesh.h>
#include <maya\MStringArray.h>


MStatus mayaUvInfo::doIt(const MArgList & argList)
{
	
	MStatus stat = MS::kSuccess;
	
	MSelectionList selection;
	MGlobal::getActiveSelectionList(selection);

	MDagPath dagpath;
	MObject componant;

	unsigned int i;
	int fIndex, vIndex;
	MString txt;

	MItSelectionList iter(selection);
	cout << "I will printing from cout ";

	for (; !iter.isDone(); iter.next())
	{
		iter.getDagPath(dagpath, componant);
		MFnMesh meshFn(dagpath, &stat);	
		if (!stat)
			continue;

		txt += MString("object: ") + dagpath.fullPathName()+ "\n";
		MStringArray uvSetNames;
		meshFn.getUVSetNames(uvSetNames);

		for (i = 0; i < uvSetNames.length(); i++)
		{
			txt +=	MString(" UV Set: ") + uvSetNames[i] +"\n";
			txt += MString("# UVs: ") + meshFn.numUVs(uvSetNames[i] +"\n");
		}

		MString cUVSetName;
		meshFn.getCurrentUVSetName(cUVSetName);
		MItMeshVertex vertIter(dagpath, componant, &stat);
		if (!stat)
			continue;

		float2 vUV;
		MFloatArray fvUs, fvVs;
		MIntArray faceIdS, vertIds;
		unsigned int fvIndex;

		for (; !vertIter.isDone(); vertIter.next())
		{
			vIndex = vertIter.index();
			txt += MString(" Vertex: ") + vIndex + "\n";
			bool hasUV = false;

			stat = vertIter.getUV(vUV,&cUVSetName);
			if (stat)
			{
				txt += MString("Vertex UV: ") + vUV[0] + "," + vUV[1] + "\n";
				hasUV = true;
			}
			
			stat = vertIter.getUVs(fvUs, fvVs, faceIdS, &cUVSetName);
			if (stat)
			{
				for (i = 0; i < faceIdS.length(); i++)
				{
					fIndex = faceIdS[i];
					meshFn.getPolygonVertices(fIndex, vertIds);
					for (fvIndex = 0; fvIndex < vertIds.length(); fvIndex++)
					{
						txt += MString("Face-vertex UV: face,vertex:") + fIndex + "," + fvIndex + "," + fvIndex + " UV: " + fvUs[i] + fvVs[i] + "\n";
					}
					hasUV = true;					
				}
				if (!hasUV)
				{
					txt += "NO Assigned UVS \n";
				}
			}
			
		}
		
	}

	MGlobal::displayInfo(txt);
	return MS::kSuccess;

}

void * mayaUvInfo::creator()
{
	return new mayaUvInfo;
}






MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "Atri Dave", "1.0", "Any");
	MStatus status = plugin.registerCommand("mayaUvInfo", mayaUvInfo::creator);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	MStatus status = plugin.deregisterCommand("mayaUvInfo");
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}