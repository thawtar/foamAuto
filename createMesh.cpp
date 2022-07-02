#include <iostream>
#include <cstdlib>
#include <sys/stat.h>
#include <cstdio>
#include <cstring>
#include "createMesh.hpp"

using namespace std;



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
    const int left[4]={0,3,7,4};
    const int right[4]={1,2,6,5};
    const int top[4]={4,5,6,7};
    const int bottom[4]={0,1,2,3};
    const int front[4]={4,5,1,0};
    const int back[4]={7,6,2,3};
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



