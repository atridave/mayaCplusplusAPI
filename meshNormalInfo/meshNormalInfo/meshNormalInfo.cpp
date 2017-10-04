#include "meshNormalInfo.h"
#include <maya\MFnPlugin.h>
#include <maya\MFnMesh.h>
#include <maya\MItMeshPolygon.h>
#include <maya\MItMeshVertex.h>

void addData(MString &txt,
			const int fIndex,
			const int vIndex,
			const int fvIndex,
			const MVector &fNormal,
			const MVector &vNormal,
			const MVector &fvNormal)
{
	txt += MString(" Face: ") + fIndex + " Vertex: " + vIndex + " Face-vertex: " + fvIndex + "\n";
	txt += MString(" FaceNormal: ") + fNormal.x + ","+fNormal.y+","+fNormal.z+ "\n";
	txt += MString(" vertexNormal: ") + vNormal.x + "," + vNormal.y + "," + vNormal.z + "\n";
	txt += MString(" face-vertexNormal: ") + fvNormal.x + "," + fvNormal.y + "," + fvNormal.z + "\n";
}


MStatus meshNormalInfo::doIt(const MArgList & argList)
{
	MStatus stat = MS::kSuccess;
	MSelectionList selection;
	MGlobal::getActiveSelectionList(selection);

	MDagPath dagPath;
	MObject component;

	unsigned int i, nVerts;
	int fIndex, vIndex, fvIndex;
	MVector fNormal, vNormal, fvNormal;
	MString txt;

	MItSelectionList iter(selection);

	for (; !iter.isDone(); iter.next())
	{
		iter.getDagPath(dagPath, component);
		MFnMesh meshfn(dagPath);

		MItMeshPolygon faceIter(dagPath, component, &stat);

		if (stat == MS::kSuccess)
		{
			txt += MString("Object : ") + dagPath.fullPathName() + "\n";

			nVerts = faceIter.polygonVertexCount();
			for (i = 0; i < nVerts; i++)
			{
				fvIndex = i;
				fIndex = faceIter.index();
				vIndex = faceIter.vertexIndex(i);

				faceIter.getNormal(fNormal);
				meshfn.getVertexNormal(vIndex, vNormal);
				faceIter.getNormal(fvIndex, fvNormal);

				addData(txt, fIndex, vIndex, fvIndex, fNormal, vNormal, fvNormal);

			}
		}

		else
		{
			MItMeshVertex vertIter(dagPath, component, &stat);

			if (stat == MS::kSuccess)
			{
				txt += MString("Object: ") + dagPath.fullPathName() + "\n";
				
				MIntArray faceIds;
				MIntArray vertIds;

				for (; !vertIter.isDone(); vertIter.next())
				{
					vIndex = vertIter.index();
					vertIter.getNormal(vNormal);
					vertIter.getConnectedFaces(faceIds);

					for (i = 0; i < faceIds.length(); i++)
					{
						fIndex = faceIds[i];
						meshfn.getPolygonNormal(fIndex,fNormal);
						meshfn.getFaceVertexNormal(fIndex, vIndex, fvNormal);
						meshfn.getPolygonVertices(fIndex, vertIds);

						for (fvIndex = 0; fvIndex < int(vertIds.length()); fvIndex ++)
						{
							if (vertIds[fvIndex] == vIndex)
								break;

						}

						addData(txt, fIndex, vIndex, fvIndex, fNormal, vNormal, fvNormal);						

					}
				}

			}
		}
	}

	
	MGlobal::displayInfo(txt);
	return MS::kSuccess;
}





void * meshNormalInfo::creator()
{
	return new meshNormalInfo;
}





MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "Atri Dave", "1.0", "Any");
	MStatus status = plugin.registerCommand("meshNormalInfo", meshNormalInfo::creator);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	MStatus status = plugin.deregisterCommand("meshNormalInfo");
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}