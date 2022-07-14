#include <iostream>
#include <string>
#include <vector>
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

