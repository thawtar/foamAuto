#include <iostream>
#include <cstdlib>
#include <sys/stat.h>
#include <fstream>
#include <cstdio>
#include <cstring>

using namespace std;

#ifndef BLOCKMESHCREATE
#define BLOCKMESHCREATE

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
    ofstream blockMeshDict;
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
    void assignBoundary(int num, int type, char*name, int*faces);
    void writeBoundary(int num);
    void writeBoundaries(); // to write all the 6 boundary patches
};

void blockMesh::writeBoundary(int num)
{
    const char* types[4] = { "wall", "patch", "symmetryPlane", "symmetry" };
    blockMeshDict << boundaries[num].name << endl;
    blockMeshDict << "\t{\n\t" << "type " << types[boundaries[num].type] << " ;\n";
    blockMeshDict << "faces\n\t(\n\t\t(" << boundaries[num].faces[0] << " " << boundaries[num].faces[1]
        << " " << boundaries[num].faces[2] << " " << boundaries[num].faces[3] << ")\n\t\t);\n\t}";
}

void blockMesh::assignBoundary(int num, int type, char* name, int* faces)
{
    strcpy(boundaries[num].name, name);
    boundaries[num].type = type;
    for (int i = 0; i < 4; i++)
        boundaries[num].faces[i] = faces[i];
}

void blockMesh::writeVertices()
{
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
    float dx, dy, dz;
    dx = lx / float(nx);
    dy = ly / float(ny);
    dz = lz / float(nz);
	printf("\n%f %f %f\n", dx, dy, dz);
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
#endif

int test()
{
    blockMesh mesh;
    mesh.showLogo();
    mesh.writeHeader();
	mesh.setDomainSize(10, 2, 1);
	mesh.setElements(10, 5, 1);
	mesh.computeVertices();
	mesh.writeVertices();
	mesh.writeBlocks();
	mesh.writeEdges();
    int faces[4] = { 3, 7, 6, 2 };
    mesh.assignBoundary(0, 0, "movingWall", faces);
    mesh.writeBoundary(0);
    return 0;
}


int main()
{
    test();
    return 0;
}

