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
    scale = 1.0;
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
    TrueFalse[0] = "false";
    TrueFalse[1] = "true";
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
    addSTL("stl1.stl", "stl", 0, 1, 3);
    addSTL("stl2.stl", "stl2", 1, 2, 5);
    addSTL("stl3.stl", "stl3", 1, 3, 3);
    printMultipleSTLs();
    //askSTL();
    writeMainControls();
    writeCastellatedControls();
    writeSnapControls();
    writeLayerControls();
    writeMeshQualityControls();
    showText();
    writeSHMFile();
    //std::cout << tempText << std::endl;
}

void snappyHexMesh::addSTL(std::string filename, std::string name, int minRef, int maxRef, int nLayers)
{
    stl = filename;
    stlSurface aSurface;
    aSurface.fileName = filename;
    aSurface.surfaceName = name;
    aSurface.minRefine = minRef;
    aSurface.maxRefine = maxRef;
    aSurface.nSurfaceLayers = nLayers;
    surfaces.push_back(aSurface);
}

void snappyHexMesh::askSTL()
{
    std::cout << "\nEnter the STL file name:" << std::endl;
    std::cin >> stl;
}

void snappyHexMesh::printSTL(stlSurface aSurface)
{
    std::cout << "\n" << aSurface.fileName << "\n" << aSurface.surfaceName << "\n"
        << "Refinement (" << aSurface.minRefine << " " << aSurface.maxRefine << ")\n";
    std::cout << "Layers: " << aSurface.nSurfaceLayers << std::endl;
}

void snappyHexMesh::printMultipleSTLs()
{
    for (auto i = surfaces.begin(); i != surfaces.end(); ++i)
    {
        printSTL(*i);
    }
}

void snappyHexMesh::writeSTL(stlSurface stl)
{
	clearTemp();
	tempText += stl.fileName;
}

void snappyHexMesh::mergeText()
{
    snappyText += tempText;
    tempText = "";
}
void snappyHexMesh::addTempText(int indx)
{
    if (indx < maxItems)
        tempText += keyWords[indx] + "\t\t" + std::to_string(itemValues[indx])+"\n";
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
    clearTemp();
    addTrueFalseItem("castellatedMesh", 1);
    addTrueFalseItem("snap", 0);
    addTrueFalseItem("addLayers", 0);
    snappyText += tempText;
}

void snappyHexMesh::writeCastellatedControls()
{
    clearTemp();
    tempText += "\ncastellatedMeshControls\n{\n";
    //tempText += "maxLocalCells\t\t" + std::to_string(maxLocalCells)+"\n";
    addItem("maxLocalCells", maxLocalCells,1);
    addItem("maxGlobalCells",maxGlobalCells,1);
    addItem("minRefinementCells",minRefinementCells,1);
    addItem("maxLoadUnbalance",maxLoadUnbalance,0);
    addItem("nCellsBetweenLevels",nCellsBetweenLevels,1);
    addItem("resolveFeatureAngle", resolveFeatureAngle,1);
    tempText += "insidePoint ("+ std::to_string(insidePointX)+" "+std::to_string(insidePointY);
    tempText += " "+std::to_string(insidePointZ)+");\n";
    tempText += "\n}\n";
    mergeText(); // add tempText to snappyText
    //std::cout<< snappyText<< std::endl;
}

void snappyHexMesh::showText()
{
    std::cout<< snappyText<< std::endl;
}

void snappyHexMesh::writeSnapControls()
{
    clearTemp();
    tempText += "\nsnapControls\n{\n";
    addItem("nSmoothPatch", nSmoothPatch,1);
    addItem("tolerance", snapTolarence,0);
    addItem("nSolverIter", nSolveIter,1);
    addItem("nRelaxIter", nRelaxIter,1);
    addItem("nFeatureSnapIter", nFeatureSnapIter,1);
    addTrueFalseItem("implicitFeatureSnap",implicitFeatureSnap);
    addTrueFalseItem("explicitFeatureSnap",explicitFeatureSnap);
    addTrueFalseItem("multiRegionFeatureSnap",multiRegionFeatureSnap);
    tempText += "\n}\n";
    mergeText(); // add tempText to snappyText
    //std::cout<< snappyText<< std::endl;
}

void snappyHexMesh::writeMeshQualityControls()
{
    clearTemp();
    tempText += "\nmeshQualityControls\n{\n";
    addItem("maxNonOrtho", maxNonOrtho,1);
    addItem("maxBoundarySkewness", maxBoundarySkewness ,1);
    addItem("maxInternalSkewness", maxInternalSkewness ,1);
    addItem("maxConcave", maxConcave ,1);
    addItem("minVol",minVol ,0);
    addItem("minTetQuality",minTetQuality ,0);
    addItem("minArea",minArea ,1);
    addItem("minTwist", minTwist,0);
    addItem("minDeterminant", minDeterminant ,0);
    addItem("minFaceWeight", minFaceWeight ,0);
    addItem("minVolRatio", minVolRatio,0);
    addItem("minTriangleTwist", minTriangleTwist,0);
    addItem("nSmoothScale", nSmoothScale,1);
    addItem("errorReduction", errorReduction,0);
    tempText += "\n}\n";
    mergeText(); // add tempText to snappyText
}


void snappyHexMesh::writeLayerControls()
{
    clearTemp();
    tempText += "\naddLayerControls\n{\n";
    addTrueFalseItem("relativeSizes",relativeSizes);
    addItem("expansinoRatio", expansionRatio,0);
    addItem("finalLayerThickness", finalLayerThickness,0);
    addItem("minThickness", minThickness,0);
    addItem("nGrow", expansionRatio,1);
    addItem("featureAngle", featureAngle,1);
    addItem("slipFeatureAngle", slipFeatureAngle,1);
    addItem("nRelaxIter", nRelaxIter,1);
    addItem("nSmoothSurfaceNormals", nSmoothSurfaceNormals,1);
    addItem("nSmoothNormals", nSmoothNormals,1);
    addItem("nSmoothThickness", nSmoothThickness,1);
    addItem("maxFaceThicknessRatio", maxFaceThicknessRatio,0);
    addItem("maxThicknessToMedialRatio",maxThicknessToMedialRatio ,0);
    addItem("minMedianAxisAngle",minMedianAxisAngle ,1);
    addItem("nBufferCellsNoExtrude", nBufferCellsNoExtrude ,1);
    addItem("nLayerIter", nLayerIter ,1);
    tempText += "\n}\n";
    mergeText(); // add tempText to snappyText
}

void snappyHexMesh::addTrueFalseItem(std::string name, int value)
{
    tempText += name + "\t" + TrueFalse[value] + ";\n";
}

void snappyHexMesh::writeSHMFile()
{
    if (!snappyHexMeshDict.is_open())
        snappyHexMeshDict.open("snappyHexMeshDict");
    snappyHexMeshDict<<snappyText<< std::endl;

}

void snappyHexMesh::addItem2(item anItem)
{
    if (anItem.isInt)
    {
        tempText += anItem.name + "\t" + std::to_string((int)(anItem.value)) + ";\n";
    }
    else
        tempText += anItem.name + "\t" + std::to_string(anItem.value) + ";\n";
}

void snappyHexMesh::addItem(std::string name, float value, int isInt)
{
    if(isInt)
        tempText += name + "\t" + std::to_string((int)(value)) + ";\n";
    else
        tempText += name + "\t" + std::to_string(value) + ";\n";
}