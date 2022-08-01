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
	boundaryCount = -1; //this is undefined flag. 
}
boundaryConditions::~boundaryConditions()
{
	//boundaryCount = bcCount;
	if (bcfile.is_open())
		bcfile.close();
		boundaryCount = -1;
}
boundaryConditions::boundaryConditions(std::string filename)
{
	clearMainText();
	for(int i=0;i<DIMENSIONS;i++)
		dimensions[i] = 0;
	bcFileName = filename;
	bcfile.open(bcFileName);
	bcClass = 0; // default value
	boundaryCount = -1; // default value
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

// There are two functions having the same name. name+value and name+string(type)
void boundaryConditions::addItem(std::string name, float value, int isInt)
{
    if(isInt)
        tempText += name + "\t" + std::to_string((int)(value)) + ";\n";
    else
        tempText += name + "\t" + std::to_string(value) + ";\n";
}

void boundaryConditions::addItem(std::string str1, std::string str2)
{
	tempText += "\t"+str1+"\t"+str2;
}

void boundaryConditions::writeBoundaryCondition(boundaryCondition bc)
{
	const std::string bcTypes[]={"empty","fixedValue", "zeroGradient", "symmetry","nonSlip","fixedFluxPressure"};
	clearTemp();
	addText("\n");
	addText(bc.name);
	addText("\n  {\n");
	std::string bcValueString;
	addItem("type", bcTypes[bc.bcType]);
	if(bc.scalarVector==Scalar)
	{
		addItem("\n\tvalue\tuniform",std::to_string(bc.scalarValue));
		addText(";");
	}
	if(bc.scalarVector==Vector)
	{
		bcValueString = "\nvalue\tuniform("+std::to_string(bc.valueX)+" "+std::to_string(bc.valueY)+" "+std::to_string(bc.valueZ)+");\n";
		addText(bcValueString);
	}
	addText("\n  }");
	addTextToMain();
}

void boundaryConditions::write_multiple_boundary_conditions()
{
	// iterate over the whole boundary conditions vector
	for(auto iterator=bcs.begin();iterator!=bcs.end();++iterator)
	{
		writeBoundaryCondition(*iterator);
	}
}

void boundaryConditions::addVectorBoundaryCondition(const std::string name, int bcType,float x,float y, float z)
{
	const std::string bcTypes[]={"empty","fixedValue", "zeroGradient", "symmetry","nonSlip","fixedFluxPressure"};
	boundaryCondition aBc;
	aBc.bcType = bcType;
	aBc.name = name;
	aBc.scalarValue = 0; // it is vector boundary condition
	aBc.valueX = x;
	aBc.valueY = y;
	aBc.valueZ = z;
	aBc.indx = 0;
	aBc.scalarVector = Vector; //vector
	bcs.push_back(aBc);
}

void boundaryConditions::addScalarBoundaryCondition(const std::string name, int bcType,float value)
{
	const std::string bcTypes[]={"empty","fixedValue", "zeroGradient", "symmetry","nonSlip","fixedFluxPressure"};
	boundaryCondition aBc;
	aBc.bcType = bcType;
	aBc.name = name;
	aBc.scalarValue = value; // it is vector boundary condition
	aBc.indx = 0;
	aBc.scalarVector = Scalar; //scalar value
	bcs.push_back(aBc);
}


void boundaryConditions::clearTemp()
{
	tempText = "";
}
void boundaryConditions::addTextToMain()
{
	bcText += tempText;
	clearTemp();
}

void boundaryConditions::run()
{
	writeHeader();
	setObjectType("U");
	setDim(1,1);
	setDim(-1,2);
	addScalarBoundaryCondition("inlet",fixedValue,10.1);
	addScalarBoundaryCondition("outlet",zeroGradient,0);
	addVectorBoundaryCondition("inlet",fixedValue,1,0,0);
	addVectorBoundaryCondition("outlet",zeroGradient,0,0,0);
	addVectorBoundaryCondition("sides",symmetry,0,0,0);
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

// this is just a glue code to connect all the parts to write out the boundary field
void boundaryConditions::write_boundaryField()
{
	clearTemp();
	addText("\nboundaryField\n{\n");
	addTextToMain();
	write_multiple_boundary_conditions();
	// add each boundary condition here
	addText("\n}\n");
	addTextToMain();
}

void boundaryConditions::inputScalarBoundaryCondition()
{
	std::string name;
	int BCType;
	float value;
	std::cout<<"Enter Boundary Condition\n";
	std::cout<<"Enter patch name: ";
	std::cin>>name;
	std::cout<<"\nEnter Boundary condition type [0:empty, 1:fixedValue, 2:zeroGradient, 3:symmetry]:";
	std::cin>>BCType;
	if(BCType==1)
	{
		std::cout<<"\nEnter value of boundary condition: ";
		std::cin>>value;
	}
	else
		value = 0;
	addScalarBoundaryCondition(name,BCType,value);
}

void boundaryConditions::inputVectorBoundaryCondition()
{
	std::string name;
	int BCType;
	float valuex, valuey, valuez;
	std::cout<<"Enter Boundary Condition\n";
	std::cout<<"Enter patch name: ";
	std::cin>>name;
	std::cout<<"\nEnter Boundary condition type [0:empty, 1:fixedValue, 2:zeroGradient, 3:symmetry]:";
	std::cin>>BCType;
	if(BCType==1)
	{
		std::cout<<"\nEnter value of boundary condition: ";
		std::cin>>valuex>>valuey>>valuez;
	}
	else
	{
		valuex = 0;
		valuey = 0;
		valuez = 0;
	}
	addVectorBoundaryCondition(name,BCType,valuex,valuey,valuez);
}

void boundaryConditions::inputMultipleBoundaryConditions()
{
	int vectScalar=0;
	if(boundaryCount=-1) // if there is no boundary count defined, yet
	{
		std::cout<<"\nEnter Boundary Count: ";
		std::cin >> boundaryCount;
	}
	if(boundaryCount<=0) // if the input was wrong
	{
		std::cout<<"\nBoundary count not properly typed!!!\nExiting...\n";
		exit(-1);
	}
	std::cout<<"\nEnter Boundary Condition Type[0:Scalar, 1:Vector]: ";
	std::cin >> vectScalar;
	for(int i=0;i<boundaryCount;i++)
	{
		if(!vectScalar)
			inputScalarBoundaryCondition();
		else
			inputVectorBoundaryCondition();
	}

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

