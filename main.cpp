#include <iostream>
#include "createMesh.hpp"
using namespace std;
float x_1, x_2, y_1, y_2, z_1, z_2;
int nx, ny, nz;
float lx, ly, lz;

int test()
{
	blockMesh mesh;
	mesh.showLogo();
	mesh.writeHeader();
	mesh.setDomainSize(lx, ly, lz);
	mesh.setElements(nx, ny, nz);
	mesh.computeVertices();
	mesh.writeVertices();
	mesh.writeBlocks();
	mesh.writeEdges();
	int faces[4] = { 3, 7, 6, 2 };
	mesh.assignBoundary(0, 0, "movingWall", faces);
	mesh.writeBoundaries();
	//mesh.writeBoundary(0);
	return 0;
}

void ask_data()
{
	cout << "\nEnter (x1, y1, z1):";
	cin >> x_1 >> y_1 >> z_1;
	cout << "\nEnter (x2, y2, z2):";
	cin >> x_2 >> y_2 >> z_2;
	cout << "\nEnter (nx, ny, nz):";
	cin >> nx >> ny >> nz;
	lx = x_2 - x_1;
	ly = y_2 - y_1;
	lz = z_2 - z_1;
}


int main()
{

	ask_data();
	test();
	return 0;
}
