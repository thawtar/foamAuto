#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#ifndef CREATECASE_H
#define CREATECASE_H
// this program is to create an OpenFOAM case directory from scratch
// inputs may include solver name, turbulence on/off, free-surface on/off

// It is convenient to define values of types like fixedValue, zeroGradient or symmetry
#define empty 0
#define fixedValue 1
#define zeroGradient 2
#define symmetry 3
#define noSlip 4
#define fixedFluxPressure 5

// types
#define uniform 0
#define Scalar 0
#define Vector 1

// field types for boundary conditions
#define volScalarField 0
#define volVectorField 1
#define surfaceScalarField 2
#define surfaceVectorField 3

#define Scalar 0
#define Vector 1

// we have 7 dimensions [kg m s K mol A cd]
#define DIMENSIONS 7

/* better not use unnecessary struct here
struct dimensions
{
	int kg;
	int m;
	int sec;
	int Kelvin;
}typedef dimensions;
*/
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
	int scalarVector;
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
	int dimensions[7]; // for the dimension of the variable
	void writeBoundaryCondition(boundaryCondition bc);
	void addScalarBoundaryCondition(const std::string name, int bcType,float value);
	void addVectorBoundaryCondition(const std::string name, int bcType,float x,float y, float z);
	void addItem(std::string name, float value, int isInt);
	void addItem(std::string str1, std::string str2);
	void addText(std::string str);
	void writeHeader();

	// These functions will change/set the values of the members of boundaryCondtions object
	void setBCClass(int BCClass); // to set whether it is a volVectorField, volScalarField, etc
	void setObjectType(std::string objType);
	void setBCFile(std::string filename);
	void setBCCount(int bcCount);
	void setDim(int value, int indx);


	// This part just writes down the boundary condition based on
	// the data contained in this class.
	// Boundary condition can be divided into 5 parts:
	// 1. Header, 2. FoamFile, 3. dimensions 4. internalField
	// 5. boundaryField.
	void write_foamFile();
	void write_dimensions();
	void write_internalField();
	void write_boundaryField();
	void write_multiple_boundary_conditions();
	void showText();
	void clearTemp();
	void addTextToMain(); // to add tempText to bcText
	void clearMainText();
public:
	boundaryConditions();
	boundaryConditions(int bcCount);
	boundaryConditions(std::string filename);
	boundaryConditions(std::string filename, int bcCount);
	~boundaryConditions();
	void inputScalarBoundaryCondition();
	void inputVectorBoundaryCondition();
	void inputMultipleBoundaryConditions();
	void run();
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
	std::string casepath;
public:
	createcase(); // constructor
	createcase(std::string name);
	~createcase(); // destructor
	void createDirectory();
	void run();

};



#endif
