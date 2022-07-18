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
	boundaryCount = bcCount;
	bcClass = 0; // default value for boundary condition class
}

boundaryConditions::boundaryConditions()
{
	//boundaryCount = bcCount;
}

boundaryConditions::~boundaryConditions()
{
	//boundaryCount = bcCount;
	if (bcfile.is_open())
		bcfile.close();
}

boundaryConditions::boundaryConditions(std::string filename)
{
	bcFileName = filename;
	bcfile.open(bcFileName);
}

boundaryConditions::boundaryConditions(std::string filename, int bcCount)
{
	bcFileName = filename;
	bcfile.open(bcFileName);
	boundaryCount = bcCount;
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

void boundaryConditions::run()
{
	writeHeader();
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
	addText("\tformat\tascii;\nclass\t");
}
void boundaryConditions::write_dimensions()
{

}
void boundaryConditions::write_internalField()
{

}
void boundaryConditions::write_boundaryField()
{

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

