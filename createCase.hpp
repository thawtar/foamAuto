#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#ifndef CREATECASE_H
#define CREATECASE_H
// this program is to create an OpenFOAM case directory from scratch
// inputs may include solver name, turbulence on/off, free-surface on/off

class createcase
{
private:
	std::string casename;
	std::string path;
public:
	createcase(); // constructor
	createcase(std::string name);
	~createcase(); // destructor
	void run();

};



#endif
