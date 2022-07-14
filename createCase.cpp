#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <filesystem> // should i use it or not?
#include "createCase.hpp"


// Constructor with the number of boundaries defined
boundaryConditions::boundaryConditions(int bcCount)
{
	boundaryCount = bcCount;
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
	clearTemp();
	
}

void boundaryConditions::clearTemp()
{
	tempText = "";
}


void createcase::createDirectory()
{
	std::string mkdircmd = "mkdir -p "+path;
	if()
	std::system(mkdircmd.c_str());
}


