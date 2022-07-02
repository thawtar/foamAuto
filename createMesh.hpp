#include <iostream>
#include <fstream>
#ifndef CREATEMESH_H
#define CREATEMESH_H

struct boundary
{
    char name[20];
    int type;
    int faces[4];

}typedef boundary;

class blockMesh
{
private:
    int nx, ny, nz; // the number of elements in each domain direction
    //FILE* blockmeshdict;
    std::ofstream blockMeshDict;
    float lx, ly, lz; // size of the block mesh domain
    float scale;
    float x[8], y[8], z[8]; // for the vertices of the domain
    boundary boundaries[6];
public:
    blockMesh();
    blockMesh(float x0, float y0, float z0);
    ~blockMesh();
    void showLogo();
    int writeHeader();
    void setDomainSize(float a, float b, float c);
    void setElements(int ax, int ay, int az);
    void computeVertices();
	void writeVertices();
	void writeBlocks();
	void writeEdges();
    void assignBoundary(int num, int type, const char*name, int*faces);
    void writeBoundary(int num);
    void writeBoundaries(); // to write all the 6 boundary patches
};


class snappyHexMesh
{
private:
    std::ofstream shmDict; // snappyHexMeshDict file
    // Parameters used in snappyHexMeshDict file
    int minRef, maxRef; // minimum and maximum refinements
    char stl[100]; // store STL file name
    // Castellated mesh control parameters
    int maxLocalCells, maxGlobalCells, minRefinementCells;
    float maxLoadUnbalance;
    int nCellsBetweenLevels;
    int featureRefinementLevel;
    int minSurfaceRefinement, maxSurfaceRefinement;
    int resolveFeatureAngle;
    float insidePointX, insidePointY, insidePointZ;
    int allowFreeStandingZoneFaces;

    // Snap Controols
    int nSmoothPatch, nSolveIter, nRelaxIter;
    float snapTolarence;
    int nFeatureSnapIter, implicitFeatureSnap, explicitFeatureSnap;
    int multiRegionFeatureSnap;
    
    // Add layer control parameters
    int nSurfaceLayers;
    int relativeSizes;
    float expansionRatio;
    float finalLayerThickness, minThickness;
    int nGrow, featureAngle, slipFeatureAngle;
    int nSnapRelaxIter, nSmoothSurfaceNormals;
    int nSmoothNormals, nSmoothThickness;
    float maxFaceThicknessRatio, maxThicknessToMedialRatio;
    int minMedianAxisAngle, nBufferCellsNoExtrude, nLayerIter;


    // Mesh Quality Controls

    // Write Flags

    float mergeTolerance;

public:
    snappyHexMesh();
    ~snappyHexMesh();
    int writeHeader();
    void searchableBox(char name[100],float xx1, float xx2, float yy1, float yy2,
    float zz1, float zz2);

    void run();
};
#endif // end of CREATEMESH
