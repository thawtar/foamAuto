#include <iostream>
#include <fstream>
#include <string>
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
    std::ofstream snappyHexMeshDict; // snappyHexMeshDict file
    std::string snappyText;
    std::string tempText; // to temporarily store string before adding to snappyText
    std::string keyWords[100] = { "maxLocalCells","maxGlobalCells", "minRefinementCells","minRefinementCells",
    "maxLoadUnbalance", "nCellsBetweenLevel","featureRefinementLevel" };
    float defaultValues[100] = { 100000.0,2.0e+6,10.0,0.1,3, 6 };
    // Parameters used in snappyHexMeshDict file
    int minRef, maxRef; // minimum and maximum refinements
    std::string stl; // store STL file name
    // Main step switches
    int castellatedMesh, snap, addLayers;
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
    int maxNonOrtho, maxBoundarySkewness, maxInternalSkewness;
    int maxConcave;
    float minVol, minTetQuality, minArea, minTwist, minDeterminant;
    float minFaceWeight, minVolRatio, minTriangleTwist;
    int nSmoothScale;
    float errorReduction;
    int relaxedMaxNonOrtho;

    // Write Flags

    float mergeTolerance;
    void changeValueInt(int* variableToBeChanged, int value);
    void changeValueFloat(float* variableToBeChanged, float value);
	void mergeText();
	void addTempText(int indx);

public:
    snappyHexMesh();
    ~snappyHexMesh();
    int writeHeader();
    void inputSTL(std::string name);
    void askSTL();
    void searchableBox(char name[100],float xx1, float xx2, float yy1, float yy2,
    float zz1, float zz2);

    void run();
};
#endif // end of CREATEMESH
