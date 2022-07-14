#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#ifndef CREATECASE_H
#define CREATECASE_H
// this program is to create an OpenFOAM case directory from scratch
// inputs may include solver name, turbulence on/off, free-surface on/off

// It is convenient to define values of types like fixedValue, zeroGradient or symmetry
#define empty -1;
#define fixedValue 0;
#define zeroGradient 1;
#define symmetry 2;
#define noSlip 3;
#define fixedFluxPressure 4;

// types
#define uniform 0;
#define Scalar 0;
#define Vector 1;


struct dimensions
{
	int kg;
	int m;
	int sec;
	int Kelvin;
}typedef dimensions;

struct transportProperties
{
	float rho;
	float nu;
}typedef transportProperties;

// to store data for each boundary condition for each patch
struct boundaryCondition
{
	std::string name; // name of the boundary
	int indx;
	int bcType; // type of boundary condition. fixedValue, zeroGradient or symmetry
	int valueType; // type of value: scalar or vector
	float valueX, valueY, valueZ; // in case of defining vector values
	float scalarValue; // in case of defining scalar values
};

// It is necessary to create a new instance of this class for each boundary condition file
class boundaryConditions
{
private:
	std::ofstream bcfile;
	std::string bcText;
	std::string tempText;
	int boundaryCount;
	int bcClass; // whether it is volVectorField, volScalarField
	std::string objectType;
	std::string bcFileName; // name of the boundary condition file
	std::vector<boundaryCondition> bcs;
	void addBoundaryCondition(boundaryCondition bc);
	void addItem(std::string name, float value, int isInt);
	void writeHeader();
	void clearTemp();

public:
	boundaryConditions();
	boundaryConditions(int bcCount);
	boundaryConditions(std::string filename);
	boundaryConditions(std::string filename, int bcCount);
	~boundaryConditions();
	

};

class constants
{
private:
	int turbulence;
	int gravity;
	int transportProperties;
};


class createcase
{
private:
	std::string casename;
	std::string path;
public:
	createcase(); // constructor
	createcase(std::string name);
	~createcase(); // destructor
	void createDirectory();
	void run();

};



#endif
