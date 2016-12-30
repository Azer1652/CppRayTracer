/*
 * Parser.h
 *
 *  Created on: 4 Oct 2016
 *      Author: arthu
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <fstream>
#include <iostream>
#include <string>
//#include <sstream>
#include <stdlib.h>

class Scene;
class Material;
class Bar;
class Sphere;
class BoolObj;

class Parser {
public:
	Parser(Scene*, std::string);
	virtual ~Parser();
private:
	int line;
	Scene* scene;

	std::ifstream infile;
	std::string sCurrentLine;

	void parseFile(std::string);
	void readObjects();
	void readCamera();
	void readLight();
	BoolObj* readBool();
	Bar* readBar();
	Sphere* readSphere();
	//void readSphere();
	//void readCone();

	void setCamLocation();
	void setCamUp();
	void setCamLookAt();

	double* readABC();
	double* readPos();
	double* readColor();
	Material* readMat();

};

#endif /* PARSER_H_ */
