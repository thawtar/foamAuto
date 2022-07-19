#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <filesystem> // should i use it or not?
#include <sys/types.h>
#include "createCase.hpp"


// Constructor with the number of boundaries defined
boundaryConditions::boundaryConditions(int bcCount)
{
	clearMainText();
	for(int i=0;i<DIMENSIONS;i++)
		dimensions[i] = 0;
	boundaryCount = bcCount;
	bcClass = 0; // default value for boundary condition class
}
boundaryConditions::boundaryConditions()
{
	//boundaryCount = bcCount;
	clearMainText();
	for(int i=0;i<DIMENSIONS;i++)
		dimensions[i] = 0;
	bcClass = 0; // default value
	boundaryCount = 1; // default value
}
boundaryConditions::~boundaryConditions()
{
	//boundaryCount = bcCount;
	if (bcfile.is_open())
		bcfile.close();
}
boundaryConditions::boundaryConditions(std::string filename)
{
	clearMainText();
	for(int i=0;i<DIMENSIONS;i++)
		dimensions[i] = 0;
	bcFileName = filename;
	bcfile.open(bcFileName);
	bcClass = 0; // default value
	boundaryCount = 1; // default value
}
boundaryConditions::boundaryConditions(std::string filename, int bcCount)
{
	clearMainText();
	for(int i=0;i<DIMENSIONS;i++)
		dimensions[i] = 0;
	bcFileName = filename;
	bcfile.open(bcFileName);
	bcClass = 0; // default value
	boundaryCount = bcCount;
}

void boundaryConditions::setBCClass(int BCClass) // to set whether it is a volVectorField, volScalarField, etc
{

	bcClass = BCClass;
}
void boundaryConditions::setObjectType(std::string objType)
{
	objectType = objType;
}

void boundaryConditions::setBCFile(std::string filename)
{
	if(!bcfile.is_open())
		bcfile.open(filename);
}

void boundaryConditions::setBCCount(int bcCount)
{
	if(bcCount>0)
		boundaryCount = bcCount;
}

void boundaryConditions::setDim(int value, int indx)
{
	if(indx >=DIMENSIONS)
	{
		std::cout << "Error: index overflow in dimensions\n";
		exit(-1);
	}
	dimensions[indx]=value;
}

void boundaryConditions::writeHeader()
{
	tempText = "/*--------------------------------*- C++ -*----------------------------------*\\\n \
| =========                |                                                 | \n \
| \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           | \n \
|  \\    /   O peration     | Version:  v2012                                 | \n \
|   \\  /    A nd           | Website:  www.openfoam.com                      | \n \
|    \\/     M anipulation  |                                                 | \n \
\\*---------------------------------------------------------------------------*/ \n \
";
bcText += tempText;
}

void boundaryConditions::addItem(std::string name, float value, int isInt)
{
    if(isInt)
        tempText += name + "\t" + std::to_string((int)(value)) + ";\n";
    else
        tempText += name + "\t" + std::to_string(value) + ";\n";
}

void boundaryConditions::addBoundaryCondition(boundaryCondition bc)
{
	const std::string bcTypes[]={"empty","fixedValue", "zeroGradient", "symmetry","nonSlip","fixedFluxPressure"};
	clearTemp();
	
}

void boundaryConditions::addItem(std::string str1, std::string str2)
{
	tempText += str1+"\t"+str2;
}
void boundaryConditions::clearTemp()
{
	tempText = "";
}
void boundaryConditions::addTextToMain()
{
	bcText += tempText;
}

void boundaryConditions::run()
{
	writeHeader();
	setObjectType("U");
	setDim(1,1);
	setDim(-1,2);
	write_foamFile();
	write_dimensions();
	write_internalField();
	write_boundaryField();
	showText();
}

void boundaryConditions::showText()
{
	std::cout << bcText << std::endl;
}

void boundaryConditions::addText(std::string str)
{
	tempText+=str;
}

void boundaryConditions::write_foamFile() 
// to write FoamFile header
{
	clearTemp();
	addText("\nFoamFile\n{\n\tversion\t2.0;\n");
	addText("\tformat\tascii;\n\tclass\t");
	switch (bcClass){
		case volScalarField:
			addText("volScalarField");
			break;
		case volVectorField:
			addText("volVectorField");
			break;
		case surfaceScalarField:
			addText("surfaceScalarField");
			break;
		case surfaceVectorField:
			addText("surfaceVectorField");
			break;
		default:
			std::cout<<"Error, wrong field type chosen"<< std::endl;
			exit(-1);
	}
	addText(";\n\tobject\t");
	addText(objectType);
	addText(";\n}\n");
	addTextToMain();

}
void boundaryConditions::write_dimensions()
{
	clearTemp();
	std::string tmp;
	addText("\ndimensions\t[");
	for(int i=0;i<DIMENSIONS;i++)
	{
		tmp = std::to_string(dimensions[i])+" ";
		addText(tmp);
	}
	addText("];\n");
	addTextToMain();
}
void boundaryConditions::clearMainText()
{
	bcText = "";
}
void boundaryConditions::write_internalField()
{
	clearTemp();
	addText("\ninternalField\t");
	if(bcClass==surfaceVectorField||bcClass==volVectorField)
		addText("uniform (0 0 0);\n");
	else
	{
		/* code */
		addText("uniform 0;\n");
	}
	addTextToMain();
}
void boundaryConditions::write_boundaryField()
{
	clearTemp();
	addText("\nboundaryField\n{\n");
	// add each boundary condition here
	addText("\n}\n");
	addTextToMain();
}

// clang doesnt accept C++17 standard filesystem.
// So need to change it later+-
/*
void createcase::createDirectory()
{
	std::filesystem::path fpath = casepath;

	std::string mkdircmd = "mkdir -p "+casepath;
	if(std::filesystem::is_directory(fpath))
	{
		; // do nothing
	}
	else
	{
		std::system(mkdircmd.c_str());
	}
	
}*/

/*
-------------------------------------
---------------------------------/   /
                                /   /
							   /   /
*/

