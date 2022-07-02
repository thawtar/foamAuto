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
    int layers;
    int minRef, maxRef; // minimum and maximum refinements
public:
    snappyHexMesh();
    ~snappyHexMesh();
    void run();
};
#endif // end of CREATEMESH
