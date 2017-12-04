#include "molecule2.h"
#include <maya\MFnPlugin.h>



const char *radiusFlag = "-r", *radiusLongFlag = "-radius";
const char *segsFlag = "-s", *segsLongFlag = "-segments";
const char *ballRatioFlag = "-br", *ballRatioLongFlag = "-ballRatio";



MSyntax molecule2Cmd::newSyntax()
{
	MSyntax syntax;

	syntax.addFlag(radiusFlag, radiusLongFlag, MSyntax::kDistance);
	syntax.addFlag(segsFlag, segsLongFlag, MSyntax::kLong);
	syntax.addFlag(ballRatioFlag, ballRatioLongFlag, MSyntax::kDouble);

	syntax.enableQuery(false);
	syntax.enableEdit(false);

	return syntax;
}



MStatus molecule2Cmd::doIt(const MArgList &args)
{
	MStatus stat;


	//Default values

	radius.setValue(0.1);
	segs = 6;
	ballRodRatio = 2.0;
	selMeshes.clear();

	//get values  from flags and set it to variables

	MArgDatabase argData(syntax(), args, &stat);
	if (!stat)
		return stat;

	if (argData.isFlagSet(radiusFlag))
	{
		argData.getFlagArgument(radiusFlag, 0, radius);

	}
	if (argData.isFlagSet(segsFlag))
	{
		argData.getFlagArgument(segsFlag, 0, segs);
	}
	if (argData.isFlagSet(ballRatioFlag))
	{
		argData.getFlagArgument(ballRatioFlag, 0, ballRodRatio);
	}

	

	//get slection mesh and append it to selMeshes as MDagPathArray 

	MSelectionList selection;
	MGlobal::getActiveSelectionList(selection);

	MDagPath dagPath;
	MItSelectionList iter(selection, MFn::kMesh);

	for (; !iter.isDone(); iter.next())
	{
		iter.getDagPath(dagPath);
		selMeshes.append(dagPath);
	}

	if (selMeshes.length() == 0)
	{
		MGlobal::displayWarning("select one or more meshes");
		return MS::kFailure;
	}

	return redoIt();

}

MStatus molecule2Cmd::redoIt()
{

	MStatus stat;
	MDagPath dagPath;
	MFnMesh meshFn;

	int nBallPolys;
	MPointArray ballVerts;
	MIntArray ballPolyCounts;
	MIntArray ballPolyConnects;
	MFloatArray ballUCoords;
	MFloatArray ballVCoords;
	MIntArray ballFvUVIDs;

	genBall(MPoint::origin, ballRodRatio*radius.value(), segs, nBallPolys, ballVerts, ballPolyCounts, ballPolyConnects,true,ballUCoords,ballVCoords,ballFvUVIDs);

	unsigned int i, j, vertOffest;
	MPointArray meshVerts;
	MPoint p0, p1;
	MObject objTransform;

	int nRodPolys;
	MPointArray rodVerts;
	MIntArray rodPolyCounts;
	MIntArray rodPolyConnects;
	MFloatArray rodUCoords;
	MFloatArray rodVCoords;
	MIntArray rodFvUVIDs;

	int nNewPolys;
	MPointArray newVerts;
	MIntArray newPolyCounts;
	MIntArray newPolyConnects;
	MFloatArray newUCoords;
	MFloatArray newVCoords;
	MIntArray newFvUVIDs;

	int uvOffset;
	MDagModifier dagMod;
	MFnDagNode dagFn;

	objectTransforms.clear();
	unsigned int mi;

	for (mi = 0; mi < selMeshes.length(); mi++)
	{
		dagPath = selMeshes[mi];
		meshFn.setObject(dagPath);

		uvOffset = 0;
		nNewPolys = 0;
		newVerts.clear();
		newPolyCounts.clear();
		newPolyConnects.clear();
		newUCoords.clear();
		newVCoords.clear();
		newFvUVIDs.clear();

		meshFn.getPoints(meshVerts);
		for (i = 0; i < meshVerts.length(); i++)
		{
			vertOffest = newVerts.length();
			nNewPolys += nBallPolys;

			for (j = 0; j < ballVerts.length(); j++)
				newVerts.append(meshVerts[i] + ballVerts[j]);

			for (j = 0; j < ballPolyCounts.length(); j++)
				newPolyCounts.append(ballPolyCounts[j]);

			for (j = 0; j < ballPolyConnects.length(); j++)
				newPolyConnects.append(vertOffest + ballPolyConnects[j]);

			if (i==0)
			{
				for (j = 0; j < ballUCoords.length(); j++)
				{
					newUCoords.append(ballUCoords[j]);
					newUCoords.append(ballVCoords[j]);
				}
			}

			for (j= 0; j < ballFvUVIDs.length(); j++)
			{
				newFvUVIDs.append(uvOffset + ballFvUVIDs[j]);
			}
		}

		uvOffset = newUCoords.length();


		int nRods = 0;
		MItMeshEdge eageIter(dagPath);
		for (; !eageIter.isDone(); eageIter.next(),nRods++)
		{
			p0 = eageIter.point(0,MSpace::kWorld);
			p1 = eageIter.point(1,MSpace::kWorld);

			genRod(p0, p1, radius.value(), segs, nRodPolys, rodVerts, rodPolyCounts, rodPolyConnects,nRods==0, rodUCoords, rodVCoords, rodFvUVIDs);
			vertOffest = newVerts.length();
			nNewPolys += nRodPolys;

			for (i = 0; i < rodVerts.length(); i++)
				newVerts.append(rodVerts[i]);

			for (i = 0; i < rodPolyCounts.length(); i++)
				newPolyCounts.append(rodPolyCounts[i]);

			for (i = 0; i < rodPolyConnects.length(); i++)
				newPolyConnects.append(vertOffest + rodPolyConnects[i]);


			if (nRods == 0)
			{
				for (i = 0; i < rodUCoords.length(); i++)
				{
					newUCoords.append(rodUCoords[i]);
					newUCoords.append(rodVCoords[i]);
				}
			}

			for (i = 0; i < rodFvUVIDs.length(); i++)
			{
				newFvUVIDs.append(uvOffset + rodFvUVIDs[j]);
			}
		}

		objTransform = meshFn.create(newVerts.length(), nNewPolys, newVerts, newPolyCounts, newPolyConnects, MObject::kNullObj, &stat);

		if (!stat)
		{
			MGlobal::displayError(MString("Unable to create Mesh :") + stat.errorString());
			return stat;
		}
		
		objectTransforms.append(objTransform);
		meshFn.assignUVs(newPolyCounts, newFvUVIDs);
		meshFn.updateSurface();
		dagFn.setObject(objTransform);
		dagFn.setName("Molecule");
		dagMod.commandToExecute(MString("sets -e -fe initialShadingGroup ") + meshFn.name());

	}

	MString cmd("select -r ");
	for (i = 0; i < objectTransforms.length(); i++)
	{
		dagFn.setObject(objectTransforms[i]);
		cmd += " " + dagFn.name();
	}
	dagMod.commandToExecute(cmd);
	return dagMod.doIt();

	}

	MStatus molecule2Cmd::undoIt()
	{
		MDGModifier dgMod;
		MFnDagNode dagFn;
		MObject child;

		unsigned int i;
		for (i = 0; i < objectTransforms.length(); i++)
		{
			dagFn.setObject(objectTransforms[i]);
			child = dagFn.child(0);
			dgMod.deleteNode(child);
			dgMod.deleteNode(objectTransforms[i]);
		}
		return dgMod.doIt();
			
	}




bool molecule2Cmd::isUndoable()
{
	return true;
}

void * molecule2Cmd::creator()
{
	return new molecule2Cmd;
}




MStatus genBall(const MPoint &center, const double radius, const unsigned int nSegs, int &nPolys, MPointArray &verts, MIntArray &polyCount, MIntArray &polyConnects, const bool genUVs, MFloatArray &uCoords, MFloatArray &vCoords, MIntArray &fvUVIDs)
{

	verts.clear();
	polyCount.clear();
	polyConnects.clear();

	if (genUVs)
	{
		uCoords.clear();
		vCoords.clear();
		fvUVIDs.clear();
	}

	int nAzimuthSegs = nSegs * 2;
	int nZenithSegs = nSegs;

	int nAzimuthPts = nAzimuthSegs;
	int nZenithPts = nZenithSegs + 1;

	double azimIncr = 2.0*M_PI / nAzimuthSegs;
	double zenInce = M_PI / nZenithSegs;

	MPoint p;
	double azimuth, zenith;
	double sinZenith;
	int azi, zeni;

	zenith = 0.0;

	for (zeni = 0; zeni < nZenithPts; zeni++, zenith += zenInce)
	{
		azimuth = 0.0;

		for (azi = 0; azi < nAzimuthPts; azi++, azimuth += azimIncr)
		{
			sinZenith = sin(zenith);
			p.x = radius*sinZenith*cos(azimuth);
			p.y = radius*cos(zenith);
			p.z = radius*sinZenith*sin(azimuth);

			verts.append(p);
		}
	}
	
	int nUCols = nAzimuthSegs + 1;
	int nVRows = nZenithSegs + 1;

	if (genUVs)
	{
		int nUVCoords = nUCols*nVRows;
		uCoords.setLength(nUVCoords);
		uCoords.clear();
		vCoords.setLength(nUVCoords);
		vCoords.clear();

		float uIncr = 1.0f / nAzimuthSegs;
		float vIncr = 1.0f / nZenithSegs;

		float u, v;
		int ui, vi;

		for (vi = 0, v = 0.0; vi < nVRows; vi++, v += vIncr)
		{
			for (ui = 0, u = 0.0; ui < nUCols; ui++, u += uIncr)
			{
				uCoords.append(u);
				vCoords.append(v);
			}
		}


	}
	
	nPolys = nAzimuthSegs*nZenithSegs;
	polyCount.setLength(nPolys);
	int i;

	for (i = 0; i < nPolys; i++)
		polyCount[i] = 4;

	for (zeni = 0; zeni < nZenithSegs; zeni++)
	{
		for (azi = 0; azi < nAzimuthSegs; azi++)
		{
			polyConnects.append(linerIndex(zeni, azi, nZenithPts, nAzimuthPts));
			polyConnects.append(linerIndex(zeni, azi + 1, nZenithPts, nAzimuthPts));
			polyConnects.append(linerIndex(zeni + 1, azi + 1, nZenithPts, nAzimuthPts));
			polyConnects.append(linerIndex(zeni + 1, azi, nZenithPts, nAzimuthPts));

			if (genUVs)
			{
				fvUVIDs.append(linerIndex(zeni, azi, nVRows, nUCols));
				fvUVIDs.append(linerIndex(zeni, azi+1, nVRows, nUCols));
				fvUVIDs.append(linerIndex(zeni+1, azi + 1, nVRows, nUCols));
				fvUVIDs.append(linerIndex(zeni+1, azi, nVRows, nUCols));
			}
		}
	}
	return MS::kSuccess;
}

MStatus genRod(const MPoint &p0, const MPoint &p1, const double radius, const unsigned int nSegs, int &nPolys, MPointArray &verts, MIntArray &polyCounts, MIntArray &polyConnects,
	const bool genUVs,MFloatArray &uCoords, MFloatArray &vCoords,MIntArray &fvUVIDS)
{
	verts.clear();
	polyCounts.clear();
	polyConnects.clear();

	if (genUVs)
	{
		uCoords.clear();
		vCoords.clear();
		fvUVIDS.clear();
	}

	unsigned int nCirclePts = nSegs;
	unsigned int nVerts = nCirclePts * 2;

	MVector vec(p1 - p0);
	MVector up(0.0, 1.0, 0.0);
	MVector xAxis, yAxies, zAxies;

	yAxies = vec.normal();
	if (up.isParallel(yAxies, 0.1))
		up = MVector(1.0, 0.0, 0.0);
	xAxis = yAxies ^ up;
	zAxies = (xAxis^yAxies).normal();
	xAxis = (yAxies^zAxies).normal();

	verts.setLength(nVerts);
	double angleIncr = 2 * M_PI / nSegs;
	double angle;
	MPoint p;
	double x, z;
	unsigned int i;

	for (i = 0, angle = 0; i < nCirclePts; i++, angle += angleIncr)
	{
		x = radius*cos(angle);
		z = radius*sin(angle);

		p = p0 + x * xAxis + z * zAxies;
		verts[i] = p;
		p += vec;
		verts[i + nCirclePts] = p;
	}

	int nUCols = nSegs + 1;
	int nVRows = 2;

	if (genUVs)
	{
		int nUVCoords = nUCols*nVRows;
		uCoords.setLength(nUVCoords);
		uCoords.clear();
		vCoords.setLength(nUVCoords);
		vCoords.clear();
		float uIncr = 1.0f / nSegs;
		float u, v;
		int ui, vi;

		for (vi = 0, v = 0.0; vi < nVRows; vi++, v += 1.0)
		{
			for (ui = 0,u = 0.0; ui < nUCols; ui++, u += uIncr)
			{
				uCoords.append(u);
				vCoords.append(v);
			}
		}

	}

	nPolys = nSegs;
	polyCounts.setLength(nPolys);

	for (i = 0; i < polyCounts.length(); i++)
		polyCounts[i] = 4;

	polyConnects.setLength(nPolys * 4);
	polyConnects.clear();
	for (i = 0; i < nSegs; i++)
	{
		polyConnects.append(linerIndex(0, i, 2, nCirclePts));
		polyConnects.append(linerIndex(0, i + 1, 2, nCirclePts));
		polyConnects.append(linerIndex(1, i + 1, 2, nCirclePts));
		polyConnects.append(linerIndex(1, i, 2, nCirclePts));

		if (genUVs)
		{
			fvUVIDS.append(linerIndex(0, i, nVRows, nUCols));
			fvUVIDS.append(linerIndex(0, i+1, nVRows, nUCols));
			fvUVIDS.append(linerIndex(1, i+1, nVRows, nUCols));
			fvUVIDS.append(linerIndex(1, i, nVRows, nUCols));
		}
	}

	return MS::kSuccess;
}



MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "Atri Dave", "1.0", "Any");
	MStatus status = plugin.registerCommand("molecule2", molecule2Cmd::creator, molecule2Cmd::newSyntax);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	MStatus status = plugin.deregisterCommand("molecule2");
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}