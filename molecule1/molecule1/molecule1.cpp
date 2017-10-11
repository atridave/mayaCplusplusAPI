#include "molecule1.h"

MStatus molecule1::doIt(const MArgList & argList)
{
	double radius = 0.1;
	int segs = 6;
	double ballRodRatio = 2.0;

	MStatus stat;

	MSelectionList selection;
	MGlobal::getActiveSelectionList(selection);

	MDagPath dagPath;
	MFnMesh meshFn;

	int nBallPolys;
	MPointArray ballVerts;
	MIntArray ballPolyCounts;
	MIntArray ballPolyConnects;

	genBall(MPoint::origin, ballRodRatio*radius, segs, nBallPolys, ballVerts, ballPolyCounts, ballPolyConnects);

	unsigned int i, j, vertOffest;
	MPointArray meshVerts;
	MPoint p0, p1;
	MObject objTransform;

	int nRodPolys;
	MPointArray rodVerts;
	MIntArray rodPolyCounts;
	MIntArray rodPolyConnects;

	int nNewPolys;
	MPointArray newVerts;
	MIntArray newPolyCounts;
	MIntArray newPolyConnects;

	MItSelectionList iter(selection, MFn::kMesh);

	for (; !iter.isDone(); iter.next())
	{
		iter.getDagPath(dagPath);
		meshFn.setObject(dagPath);

		nNewPolys = 0;
		newVerts.clear();
		newPolyConnects.clear();

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
		}

		MItMeshEdge eageIter(dagPath);
		for (; !eageIter.isDone(); eageIter.next())
		{
			p0 = eageIter.point(0);
			p1 = eageIter.point(1);

			genRod(p0, p1, radius, segs, nRodPolys, rodVerts, rodPolyCounts, rodPolyConnects);
			vertOffest = newVerts.length();
			nNewPolys += nRodPolys;

			for (i = 0; i < rodVerts.length(); i++)
				newVerts.append(rodVerts[i]);

			for (i = 0; i < rodPolyCounts.length(); i++)
				newPolyCounts.append(rodPolyCounts[i]);

			for (i = 0; i < rodPolyConnects.length(); i++)
				newPolyConnects.append(vertOffest + rodPolyConnects[i]);
			
		}
		
		objTransform = meshFn.create(newVerts.length(), nNewPolys, newVerts, newPolyCounts, newPolyConnects, MObject::kNullObj, &stat);

		if (!stat)
			stat.perror("Unable to create Mesh");

		meshFn.updateSurface();
		MString cmd("sets -e -fe initialShadingGroup");
		cmd += meshFn.name();
		MGlobal::executeCommand(cmd);
		
	}
	
	return MS::kSuccess;
}


void * molecule1::creator()
{
	return new molecule1;
}


MStatus genBall(const MPoint &center, const double radius, const unsigned int nSegs, int &nPolys, MPointArray &verts, MIntArray &polyCount, MIntArray &polyConnects)
{
	verts.clear();
	polyCount.clear();
	polyConnects.clear();

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
		}
	}
	return MS::kSuccess;
}

MStatus genRod(const MPoint &p0, const MPoint &p1, const double radius, const unsigned int nSegs, int &nPolys, MPointArray &verts, MIntArray &polyCounts, MIntArray &polyConnects)
{
	verts.clear();
	polyCounts.clear();
	polyConnects.clear();

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
	
	for (i = 0, angle = 0; i < nCirclePts ; i++, angle += angleIncr)
	{
		x =	radius*cos(angle);
		z = radius*sin(angle);

		p = p0 + x * xAxis + z * zAxies;
		verts[i] = p;
		p += vec;
		verts[i + nCirclePts] = p;
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
		polyConnects.append(linerIndex(0, i+1, 2, nCirclePts));
		polyConnects.append(linerIndex(1, i + 1, 2, nCirclePts));
		polyConnects.append(linerIndex(1, i, 2, nCirclePts));
	}
		
	return MS::kSuccess;
}

