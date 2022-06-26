#include <iostream>
#include <cstdlib>
#include <sys/stat.h>
#include <fstream>

using namespace std;

#ifndef BLOCKMESHCREATE
#define BLOCKMESHCREATE
class blockMesh
{
private:
    int nx, ny, nz; // the number of elements in each domain direction
    //FILE* blockmeshdict;
    ofstream blockMeshDict;
    float lx, ly, lz; // size of the block mesh domain
    float scale;
    float x[8], y[8], z[8]; // for the vertices of the domain
public:
    blockMesh();
    blockMesh(float x0, float y0, float z0);
    ~blockMesh();
    void showLogo();
    int writeHeader();
    void setDomainSize(float a, float b, float c);
    void setElements(int ax, int ay, int az);
    void computeVertices();
};

void blockMesh::computeVertices()
{
    float dx, dy, dz;
    dx = lx / float(nx);
    dy = ly / float(ny);
    dz = lz / float(nz);
    for (int i = 0; i < nx - 1; i++)
        x[i + 1] = x[i] + dx;
    for (int i = 0; i < ny - 1; i++)
        y[i + 1] = y[i] + dy;
    for (int i = 0; i < nz - 1; i++)
        z[i + 1] = z[i] + dz;
}

void blockMesh::showLogo(){
    cout << "Hello World\n";
    cout<<"===================================================\n\n";
    cout<<"                 Welcome to foamAuto       \n\n";
    cout<<"===================================================\n\n";
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

    return 0;
}


int main()
{
    test();
    return 0;
}

