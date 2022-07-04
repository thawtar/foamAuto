#include <iostream>
#include <cstdlib>
#include <sys/stat.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include "createMesh.hpp"

using namespace std;

//============================================================================================
//             blockMesh Functions
//============================================================================================
void blockMesh::writeBoundaries()
{
    // Vertices are counter-clockwise for each boundary face
    // Vertices: 0 1 2 3 (bottom) and 4 5 6 7 (top)
    // left boundary : x-min (0 3 7 4)
    // right boundary: x-max (1 2 6 5)
    // top boundary  : z-max (4 5 6 7)
    // bottom boundary:z-min (0 1 2 3)
    // front boundary: y-min (4 5 1 0)
    // back boundary : y-max (7 6 2 3)
    //const int left[4]={0,3,7,4};
    //const int right[4]={1,2,6,5};
    //const int top[4]={4,5,6,7};
    //const int bottom[4]={0,1,2,3};
    //const int front[4]={4,5,1,0};
    //const int back[4]={7,6,2,3};
    const char*name[6]={"left","right","top","bottom","front","back"};
    int faces[6][4]={{0,3,7,4},{1,2,6,5},{4,5,6,7},{0,1,2,3},{4,5,1,0},{7,6,2,3}};
    blockMeshDict << "boundary\n(\n";
    for(int i=0;i<6;i++)
    {
        //memcpy(&boundaries[i].face, faces[i]);
        //boundaries[i].type=1;
        //strcpy(boundaries[i].name,name[i]);
        assignBoundary(i,1,name[i],faces[i]);
    }
    for(int i=0;i<6;i++)
        writeBoundary(i);
    blockMeshDict<<");\n\n";
}

void blockMesh::writeBoundary(int num)
{
    const char* types[4] = { "wall", "patch", "symmetryPlane", "symmetry" };
    blockMeshDict <<"\t"<< boundaries[num].name << endl;
    blockMeshDict << "\t{\n\t" << " type " << types[boundaries[num].type] << " ;\n";
    blockMeshDict << "\t faces\n\t(\n\t\t (" << boundaries[num].faces[0] << " " << boundaries[num].faces[1]
        << " " << boundaries[num].faces[2] << " " << boundaries[num].faces[3]
    << " )\n\t );\n\t}\n\n";
}

void blockMesh::assignBoundary(int num, int type, const char* name, int* faces)
{
    strcpy(boundaries[num].name, name);
    boundaries[num].type = type;
    for (int i = 0; i < 4; i++)
        boundaries[num].faces[i] = faces[i];
}

void blockMesh::writeVertices()
{
    // Vertex 0: xmin, ymin, zmin
    // Vertex 1: xmax, ymin, zmin
    // Vertex 2: xmax, ymax, zmin
    // Vertex 3: xmin, ymax, zmin
    // Vertex 4: xmin, ymin, zmax
    // Vertex 5: xmax, ymin, zmax
    // Vertex 6: xmax, ymax, zmax
    // Vertex 7: xmin, ymax, zmax
	blockMeshDict << "vertices\n(\n";
	blockMeshDict << "  (" << x[0] << " " << y[0] << " " << z[0] << ")\n";
	blockMeshDict << "  (" << x[nx] << " " << y[0] << " " << z[0] << ")\n";
	blockMeshDict << "  (" << x[nx] << " " << y[ny] << " " << z[0] << ")\n";
	blockMeshDict << "  (" << x[0] << " " << y[ny] << " " << z[0] << ")\n";
	blockMeshDict << "  (" << x[0] << " " << y[0] << " " << z[nz] << ")\n";
	blockMeshDict << "  (" << x[nx] << " " << y[0] << " " << z[nz] << ")\n";
	blockMeshDict << "  (" << x[nx] << " " << y[ny] << " " << z[nz] << ")\n";
	blockMeshDict << "  (" << x[0] << " " << y[ny] << " " << z[nz] << ")\n";
	blockMeshDict << ");\n";
}

void blockMesh::computeVertices()
{
	if (nx == 1)
		x[1] = lx;
	else
		x[nx] = lx;
	if (ny == 1)
		y[1] = ly;
	else
		y[ny] = ly;
	if (nz == 1)
		z[1] = lz;
	else
		z[nz] = lz;
}

void blockMesh::showLogo(){
    cout << "Hello World\n";
    cout<<"===================================================\n\n";
    cout<<"                 Welcome to foamAuto       \n\n";
    cout<<"===================================================\n\n";
}

void blockMesh::writeBlocks()
{
	blockMeshDict << "blocks\n(\n";
	blockMeshDict << "  hex (0 1 2 3 4 5 6 7) (" << nx << " " << ny << " " << nz << ")";
	blockMeshDict << " simpleGrading (1 1 1)\n);\n\n";
}

void blockMesh::writeEdges()
{
	blockMeshDict << "edges\n(\n);\n\n";
}
// Constructor
// Create the blockMeshDict file.
// initialize x, y, z vertices of the domain to (0,0,0)
blockMesh::blockMesh()
{
    std::cout << "\nStarting blockMeshDict file creation...\n";
    blockMeshDict.open("blockMeshDict");
    x[0] = 0.0;
    y[0] = 0.0;
    z[0] = 0.0;
    lx = 1.0;
    ly = 1.0;
    lz = 1.0;
    nx = 1;
    ny = 1;
    nz = 1;
}

blockMesh::blockMesh(float x0, float y0, float z0)
{
    std::cout << "\nStarting blockMeshDict file creation...\n";
    blockMeshDict.open("blockMeshDict");
    x[0] = x0;
    y[0] = y0;
    z[0] = z0;
    lx = 1.0;
    ly = 1.0;
    lz = 1.0;
    nx = 1;
    ny = 1;
    nz = 1;
}

void blockMesh::setDomainSize(float a, float b, float c)
{
    lx = a;
    ly = b;
    lz = c;
}

int blockMesh::writeHeader()
{
    const char* ofheader = "/*--------------------------------*- C++ -*----------------------------------*\\\n \
| =========                |                                                 | \n \
| \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           | \n \
|  \\    /   O peration     | Version:  v2012                                 | \n \
|   \\  /    A nd           | Website:  www.openfoam.com                      | \n \
|    \\/     M anipulation  |                                                 | \n \
\\*---------------------------------------------------------------------------*/ \n \
";
    const char* blockMeshHeader = "FoamFile\n \
{ \n \
    version     2.0; \n \
    format      ascii; \n \
    class       dictionary;\n \
    object      blockMeshDict;\n \
}\n";
    blockMeshDict << ofheader << endl;
    blockMeshDict << blockMeshHeader << endl;
    return 0;
}

blockMesh::~blockMesh()
{ // Clean the messes 
    // if the blockMeshDict file is still open, close it
    if (blockMeshDict.is_open())
        blockMeshDict.close();
}

void blockMesh::setElements(int ax, int ay, int az)
{
    nx = ax;
    ny = ay;
    nz = az;
}

//============================================================================================
//             snappyHexMesh Functions
//============================================================================================
// Constructor for snappyHexMesh
snappyHexMesh::snappyHexMesh() // constructor, it will use default values
{
    std::cout << "\nStarting snappyHexMeshDict file creation...\n";
    // Create snappyHexMeshDict file
    snappyHexMeshDict.open("snappyHexMeshDict");
    // Free up the snappyText
    snappyText = "";
    tempText = "";
    TrueFales[]={"False","True"};
    mainControls[]={1,0,0}; // castellated True, snap False, layers False
    std::string localKeyWords[100] = {"maxLocalCells","maxGlobalCells", "minRefinementCells","minRefinementCells",
    "maxLoadUnbalance", "nCellsBetweenLevel","featureRefinementLevel" };
    float localDefaults[100]= { 100000.0,2.0e+6,10.0,0.1,3, 6 };
    for (int i = 0; i < 6; i++)
    {
        keyWords[i] = localKeyWords[i];
        itemValues[i] = localDefaults[i];
    }
    for (int i = 0; i < 6; i++)
    {
        std::cout << keyWords[i] << " :\t";
        std::cout << defaultValues[i] << std::endl;
    }
    maxItems = 6;
    // Adjust/Initializa Main steps
    castellatedMesh = 0;
    snap = 0;
    addLayers = 0;

    // Castellated mesh controls
    maxLocalCells=100000;
    maxGlobalCells=1000000; 
    minRefinementCells=5;
    maxLoadUnbalance=0.10;
    nCellsBetweenLevels=3;
    featureRefinementLevel=0;
    minSurfaceRefinement=0;
    maxSurfaceRefinement=0;
    resolveFeatureAngle=30;
    insidePointX=0.0;
    insidePointY=0.0;
    insidePointZ=0.0;
    allowFreeStandingZoneFaces=1;

    // Snap controls
    nSmoothPatch=3;
    nSolveIter=30;
    nRelaxIter=5;
    snapTolarence=2.0;
    nFeatureSnapIter=10;
    implicitFeatureSnap=0;
    explicitFeatureSnap=1;
    multiRegionFeatureSnap=0;

    // Add layer controls
    nSurfaceLayers=3;
    relativeSizes=1;
    expansionRatio=1.18;
    finalLayerThickness=0.3;
    minThickness=0.1;
    nGrow=0;
    featureAngle=60;
    slipFeatureAngle=30;
    nSnapRelaxIter=3;
    nSmoothSurfaceNormals=1;
    nSmoothNormals=3;
    nSmoothThickness=10;
    maxFaceThicknessRatio=0.5;
    maxThicknessToMedialRatio=0.3;
    minMedianAxisAngle=90;
    nBufferCellsNoExtrude=0;
    nLayerIter=50;

    // Mesh Quality controls
    maxNonOrtho=70; 
    maxBoundarySkewness=20;
    maxInternalSkewness=4;
    maxConcave=80;
    minVol=1e-10;
    minTetQuality=1e-20;
    minArea=-1.0;
    minTwist=0.05;
    minDeterminant=0.001;
    minFaceWeight=0.05;
    minVolRatio=0.01;
    minTriangleTwist=-1;
    nSmoothScale=4;
    errorReduction=0.75;
    relaxedMaxNonOrtho=75;

}

// Destructor for snappyHexMesh
snappyHexMesh::~snappyHexMesh()
{ // Clean the messes 
    // if the blockMeshDict file is still open, close it
    if (snappyHexMeshDict.is_open())
        snappyHexMeshDict.close();
}

int snappyHexMesh::writeHeader()
{
    string ofheader = "/*--------------------------------*- C++ -*----------------------------------*\\\n \
| =========                |                                                 | \n \
| \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           | \n \
|  \\    /   O peration     | Version:  v2012                                 | \n \
|   \\  /    A nd           | Website:  www.openfoam.com                      | \n \
|    \\/     M anipulation  |                                                 | \n \
\\*---------------------------------------------------------------------------*/ \n \
";
    string snappyHexMeshHeader = "FoamFile\n \
{ \n \
    version     2.0; \n \
    format      ascii; \n \
    class       dictionary;\n \
    object      snappyHexMeshDict;\n \
}\n";
    tempText = "";
    tempText += ofheader + snappyHexMeshHeader;
    snappyText += tempText;
    return 0;
}

void snappyHexMesh::changeValueFloat(float* variableToBeChanged, float value)
{
    *variableToBeChanged = value;
}

void snappyHexMesh::changeValueInt(int* variableToBeChanged, int value)
{
    *variableToBeChanged = value;
}

void snappyHexMesh::run()
{
    writeHeader();
    askSTL();
    std::cout << tempText << std::endl;
}

void snappyHexMesh::inputSTL(std::string name)
{
    stl = name;
}

void snappyHexMesh::askSTL()
{
    std::cout << "\nEnter the STL file name:" << std::endl;
    std::cin >> stl;
}

void snappyHexMesh::mergeText()
{
    snappyText += tempText;
    tempText = "";
}
void snappyHexMesh::addTempText(int indx)
{
    if (indx < maxItems)
        tempText += keyWords[indx] + "\t" + std::to_string(itemValues[indx]);
    else
    {
        std::cout << "\nError. Trying to read array over the limits. Exiting...\n";
        exit(-1);
    }

}

inline void snappyHexMesh::clearTemp()
{
    tempText="";
}

void snappyHexMesh::writeMainControls()
{
    std::string mainWords[3]={"castellatedMesh","snap","addLayers"};
    tempText = "";
    for(int i=0;i<3;i++)
        tempText += mainWords[i]+"\t"+TrueFalse[mainControls[i]]+";\n";
    snappyText += tempText;
}

void snappyHexMesh::writeCastellatedControls()
{
    clearTemp();
    tempText += "\ncastellatedMeshControls\n{\n\n";
    

}