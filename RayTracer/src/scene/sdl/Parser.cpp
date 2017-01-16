/*
 * Parser.cpp
 *
 *  Created on: 4 Oct 2016
 *      Author: arthu
 */

#include "../../../hdr/scene/sdl/Parser.h"

#include "../../../hdr/scene/Scene.h"
#include "../../../hdr/scene/Light.h"
#include "../../../hdr/scene/Bar.h"
#include "../../../hdr/scene/Sphere.h"
#include "../../../hdr/scene/Cone.h"
#include "../../../hdr/scene/Cylinder.h"
#include "../../../hdr/scene/BoolObj.h"
#include "../../../hdr/scene/materials/Material.h"
#include "../../../hdr/scene/materials/Wall.h"
#include "../../../hdr/scene/materials/Mirror.h"
#include "../../../hdr/scene/materials/Glass.h"

using namespace std;

Parser::Parser(Scene* scene, string fileName) {
	line = 1;
	this->scene = scene;

	parseFile(fileName);

}

Parser::~Parser() {
	// TODO Auto-generated destructor stub
}

void Parser::parseFile(string fileName){
	infile.open(fileName.c_str());
	getline(infile, sCurrentLine);
	line++;
	while (infile && ((sCurrentLine.find("]")) == string::npos)){
		if(sCurrentLine.find("OBJECTS[") != string::npos){
			cout << "\"OBJECTS\" detected, parsing..." << endl;
			readObjects();
		}
		getline(infile, sCurrentLine);
	    line++;
	}
	cout << "END PARSING" << endl;
}

void Parser::readObjects(){
	cout << "Reading Objects." << endl;
	getline(infile, sCurrentLine);
	line++;
	while((infile && ((sCurrentLine.find("]")) == string::npos))){
		if(sCurrentLine.find("\tCAMERA{") != string::npos){
			readCamera();
		}else if(sCurrentLine.find("\tLIGHT{") != string::npos){
			readLight();
		}else if(sCurrentLine.find("\tBAR{") != string::npos){
			this->scene->getObjects()->push_back(readBar());
		}else if(sCurrentLine.find("\tCONE{") != string::npos){
			this->scene->getObjects()->push_back(readCone());
		}else if(sCurrentLine.find("\tSPHERE{") != string::npos){
			this->scene->getObjects()->push_back(readSphere());
		}else if(sCurrentLine.find("\tCYLINDER{") != string::npos){
					this->scene->getObjects()->push_back(readCylinder());
		}else if(sCurrentLine.find("BOOL<") != string::npos){
			this->scene->getObjects()->push_back(readBool());
		}

		getline(infile, sCurrentLine);
		line++;
	}
}

void Parser::readCamera(){
	cout << "Creating Camera." << endl;
	getline(infile, sCurrentLine);
	line++;
	while((infile && ((sCurrentLine.find("}")) == string::npos))){
		if(sCurrentLine.find("\t\tPOSITION(") != string::npos){
			scene->setLocation(readPos());
		}else if(sCurrentLine.find("\t\tLOOKAT(") != string::npos){
			scene->setLookAt(readPos());
		}else if(sCurrentLine.find("\t\tUP(") != string::npos){
			scene->setUp(readPos());
		}
		getline(infile, sCurrentLine);
		line++;
	}
}

void Parser::readLight(){
	cout << "Creating Light." << endl;
	Light* light = new Light();
	getline(infile, sCurrentLine);
	line++;
	while((infile && ((sCurrentLine.find("}")) == string::npos))){
		if(sCurrentLine.find("\t\tPOSITION(") != string::npos){
			light->setPosition(readPos());
		}else if(sCurrentLine.find("\t\tCOLOR(") != string::npos){
			light->setColor(readColor());
		}
		getline(infile, sCurrentLine);
		line++;
	}
	this->scene->getLights()->push_back(light);
}

BoolObj* Parser::readBool(){
	cout << "Reading BOOL." << endl;
	GeoObj* obj[2];
	int i = 0;
	getline(infile, sCurrentLine);
	line++;
	int operation = ADD;
	if(sCurrentLine.find("SUBSTRACT") != string::npos){
		operation = SUBSTRACT;
		cout << "->SUBSTRACTING." << endl;
	}else if(sCurrentLine.find("MULTIPLY") != string::npos){
		operation = MULTIPLY;
		cout << "->MULTIPLYING." << endl;
	}else{
		operation = ADD;
		cout << "->ADDING." << endl;
	}
	while((infile && ((sCurrentLine.find(">")) == string::npos))){
		if(sCurrentLine.find("\tBAR{") != string::npos){
			obj[i] = readBar();
			i++;
		}else if(sCurrentLine.find("\tCONE{") != string::npos){
			obj[i] = readCone();
			i++;
		}else if(sCurrentLine.find("\tSPHERE{") != string::npos){
			obj[i] = readSphere();
			i++;
		}else if(sCurrentLine.find("\tCYLINDER") != string::npos){
			obj[i] = readCylinder();
			i++;
		}else if(sCurrentLine.find("\tBOOL<") != string::npos){
			obj[i] = readBool();
			i++;
		}

		getline(infile, sCurrentLine);
		line++;
	}
	BoolObj* bobj = new BoolObj(obj[0], obj[1], operation);
	return bobj;
}

Bar* Parser::readBar(){
	cout << "Creating Bar." << endl;
	getline(infile, sCurrentLine);
	line++;
	Bar* bar = new Bar();
	while((infile && ((sCurrentLine.find("}")) == string::npos))){
		if(sCurrentLine.find("\t\tDIMENSIONS(") != string::npos){
			bar->setDims(readABC());
		}else if(sCurrentLine.find("\t\tROTATION(") != string::npos){
			bar->setRot(readABC());
		}else if(sCurrentLine.find("\t\tPOSITION(") != string::npos){
			bar->setPosition(readPos());
		}else if(sCurrentLine.find("\t\tMATERIAL(") != string::npos){
			bar->setMat(readMat());
		}
		getline(infile, sCurrentLine);
		line++;
	}
	bar->generateTransform();
	bar->generateInverse();
	return bar;
}

Sphere* Parser::readSphere(){
	cout << "Creating Sphere." << endl;
	getline(infile, sCurrentLine);
	line++;
	Sphere* sphere = new Sphere();
	while((infile && ((sCurrentLine.find("}")) == string::npos))){
		if(sCurrentLine.find("\t\tDIMENSIONS(") != string::npos){
			sphere->setDims(readABC());
		}else if(sCurrentLine.find("\t\tROTATION(") != string::npos){
			sphere->setRot(readABC());
		}else if(sCurrentLine.find("\t\tPOSITION(") != string::npos){
			sphere->setPosition(readPos());
		}else if(sCurrentLine.find("\t\tMATERIAL(") != string::npos){
			sphere->setMat(readMat());
		}
		getline(infile, sCurrentLine);
		line++;
	}
	sphere->generateTransform();
	sphere->generateInverse();
	return sphere;
}

Cone* Parser::readCone(){
	cout << "Creating Cone." << endl;
	getline(infile, sCurrentLine);
	line++;
	Cone* cone = new Cone();
	while((infile && ((sCurrentLine.find("}")) == string::npos))){
		if(sCurrentLine.find("\t\tDIMENSIONS(") != string::npos){
			cone->setDims(readABC());
		}else if(sCurrentLine.find("\t\tROTATION(") != string::npos){
			cone->setRot(readABC());
		}else if(sCurrentLine.find("\t\tPOSITION(") != string::npos){
			cone->setPosition(readPos());
		}else if(sCurrentLine.find("\t\tMATERIAL(") != string::npos){
			cone->setMat(readMat());
		}
		getline(infile, sCurrentLine);
		line++;
	}
	cone->generateTransform();
	cone->generateInverse();
	return cone;
}

Cylinder* Parser::readCylinder(){
	cout << "Creating Cone." << endl;
	getline(infile, sCurrentLine);
	line++;
	Cylinder* cylinder = new Cylinder();
	while((infile && ((sCurrentLine.find("}")) == string::npos))){
		if(sCurrentLine.find("\t\tDIMENSIONS(") != string::npos){
			cylinder->setDims(readABC());
		}else if(sCurrentLine.find("\t\tROTATION(") != string::npos){
			cylinder->setRot(readABC());
		}else if(sCurrentLine.find("\t\tPOSITION(") != string::npos){
			cylinder->setPosition(readPos());
		}else if(sCurrentLine.find("\t\tMATERIAL(") != string::npos){
			cylinder->setMat(readMat());
		}
		getline(infile, sCurrentLine);
		line++;
	}
	cylinder->generateTransform();
	cylinder->generateInverse();
	return cylinder;
}

double* Parser::readABC(){
	getline(infile, sCurrentLine);
	line++;
	double* dims = new double[3]{1,1,1};
	while((infile && ((sCurrentLine.find(")")) == string::npos))){
		if(sCurrentLine.find("a=") != string::npos){
			size_t location = sCurrentLine.find("a=");
			dims[0] = atof(sCurrentLine.substr(location+2).c_str());
		}else if(sCurrentLine.find("b=") != string::npos){
			size_t location = sCurrentLine.find("b=");
			dims[1] = atof(sCurrentLine.substr(location+2).c_str());
		}else if(sCurrentLine.find("c=") != string::npos){
			size_t location = sCurrentLine.find("c=");
			dims[2] = atof(sCurrentLine.substr(location+2).c_str());
		}
		getline(infile, sCurrentLine);
		line++;
	}
	return dims;
}

double* Parser::readPos(){
	double* pos = new double[3]{1,1,1};
	getline(infile, sCurrentLine);
	line++;
	while((infile && ((sCurrentLine.find(")")) == string::npos))){
		if(sCurrentLine.find("x=") != string::npos){
			size_t location = sCurrentLine.find("x=");
			pos[0] = atof(sCurrentLine.substr(location+2).c_str());
		}else if(sCurrentLine.find("y=") != string::npos){
			size_t location = sCurrentLine.find("y=");
			pos[1] = atof(sCurrentLine.substr(location+2).c_str());
		}else if(sCurrentLine.find("z=") != string::npos){
			size_t location = sCurrentLine.find("z=");
			pos[2] = atof(sCurrentLine.substr(location+2).c_str());
		}
		getline(infile, sCurrentLine);
		line++;
	}
	return pos;
}

double* Parser::readColor(){
	double* color = new double[3]{1,1,1};
	getline(infile, sCurrentLine);
	line++;
	while((infile && ((sCurrentLine.find(")")) == string::npos))){
		if(sCurrentLine.find("r=") != string::npos){
			size_t location = sCurrentLine.find("r=");
			color[0] = atof(sCurrentLine.substr(location+2).c_str());
		}else if(sCurrentLine.find("g=") != string::npos){
			size_t location = sCurrentLine.find("g=");
			color[1] = atof(sCurrentLine.substr(location+2).c_str());
		}else if(sCurrentLine.find("b=") != string::npos){
			size_t location = sCurrentLine.find("b=");
			color[2] = atof(sCurrentLine.substr(location+2).c_str());
		}
		getline(infile, sCurrentLine);
		line++;
	}
	return color;
}

Material* Parser::readMat(){
	cout << "Creating material." << endl;
	getline(infile, sCurrentLine);
	line++;
	Material* mat = new Material();
	double* ambient = new double[3]{0,0,0};
	double* specular= new double[3]{0,0,0};
	double* diffuse= new double[3]{0,0,0};
	double* emissive= new double[3]{0,0,0};
	mat->setSpecular(specular);
	mat->setAmbient(ambient);
	mat->setDiffuse(diffuse);
	mat->setEmissive(emissive);
	if((infile && ((sCurrentLine.find(")")) == string::npos))){
		if(sCurrentLine.find("GLASS;") != string::npos){
			cout << "Glass Detected" <<endl;
			delete mat;
			mat = new Glass();
		}else if(sCurrentLine.find("WALL;") != string::npos){
			cout << "Wall Detected" <<endl;
			delete mat;
			mat = new Wall();
		}else if(sCurrentLine.find("MIRROR;") != string::npos){
			cout << "Mirror Detected" <<endl;
			delete mat;
			mat = new Mirror();
		}/*else if(sCurrentLine.find("\t\t\tCHECKERS;") != string::npos){
			cout << "Checkers Detected" <<endl;
			delete mat;
			mat = new Checkers();
		}*/
	}
	while((infile && ((sCurrentLine.find(")")) == string::npos))){
		if(sCurrentLine.find("texture=") != string::npos){
			size_t location = sCurrentLine.find("=");
			mat->readBMP(sCurrentLine.substr(location+1).c_str());
		}else if(sCurrentLine.find("reflectivity=") != string::npos){
			size_t location = sCurrentLine.find("=");
			mat->setReflectivity(atof(sCurrentLine.substr(location+1).c_str()));
		}else if(sCurrentLine.find("transparency=") != string::npos){
			size_t location = sCurrentLine.find("=");
			mat->setTransparency(atof(sCurrentLine.substr(location+1).c_str()));
		}else if(sCurrentLine.find("c=") != string::npos){
			size_t location = sCurrentLine.find("=");
			mat->setSpeedOfLight(atof(sCurrentLine.substr(location+1).c_str()));
		}else if(sCurrentLine.find("specularExponent=") != string::npos){
			size_t location = sCurrentLine.find("=");
			mat->setSpecularExponent(atof(sCurrentLine.substr(location+1).c_str()));
		}else if(sCurrentLine.find("specularFraction=") != string::npos){
			size_t location = sCurrentLine.find("=");
			mat->setSpecularFraction(atof(sCurrentLine.substr(location+1).c_str()));
		}else if(sCurrentLine.find("specularR=") != string::npos){
			size_t location = sCurrentLine.find("=");
			mat->getSpecular()[0] = (atof(sCurrentLine.substr(location+1).c_str()));
		}else if(sCurrentLine.find("specularG=") != string::npos){
			size_t location = sCurrentLine.find("=");
			mat->getSpecular()[1] = (atof(sCurrentLine.substr(location+1).c_str()));
		}else if(sCurrentLine.find("specularB=") != string::npos){
			size_t location = sCurrentLine.find("=");
			mat->getSpecular()[2] = (atof(sCurrentLine.substr(location+1).c_str()));
		}else if(sCurrentLine.find("ambientR=") != string::npos){
			size_t location = sCurrentLine.find("=");
			mat->getAmbient()[0] = (atof(sCurrentLine.substr(location+1).c_str()));
		}else if(sCurrentLine.find("ambientG=") != string::npos){
			size_t location = sCurrentLine.find("=");
			mat->getAmbient()[1] = (atof(sCurrentLine.substr(location+1).c_str()));
		}else if(sCurrentLine.find("ambientB=") != string::npos){
			size_t location = sCurrentLine.find("=");
			mat->getAmbient()[2] = (atof(sCurrentLine.substr(location+1).c_str()));
		}else if(sCurrentLine.find("diffuseR=") != string::npos){
			size_t location = sCurrentLine.find("=");
			mat->getDiffuse()[0] = (atof(sCurrentLine.substr(location+1).c_str()));
		}else if(sCurrentLine.find("diffuseG=") != string::npos){
			size_t location = sCurrentLine.find("=");
			mat->getDiffuse()[1] = (atof(sCurrentLine.substr(location+1).c_str()));
		}else if(sCurrentLine.find("diffuseB=") != string::npos){
			size_t location = sCurrentLine.find("=");
			mat->getDiffuse()[2] = (atof(sCurrentLine.substr(location+1).c_str()));
		}else if(sCurrentLine.find("emissiveR=") != string::npos){
			size_t location = sCurrentLine.find("=");
			mat->getEmissive()[0] = (atof(sCurrentLine.substr(location+1).c_str()));
		}else if(sCurrentLine.find("emissiveG=") != string::npos){
			size_t location = sCurrentLine.find("=");
			mat->getEmissive()[1] = (atof(sCurrentLine.substr(location+1).c_str()));
		}else if(sCurrentLine.find("emissiveB=") != string::npos){
			size_t location = sCurrentLine.find("=");
			mat->getEmissive()[2] = (atof(sCurrentLine.substr(location+1).c_str()));
		}else if(sCurrentLine.find("texture=") != string::npos){
			size_t location = sCurrentLine.find("=");
			mat->setTexture((atof(sCurrentLine.substr(location+1).c_str())));
		}
		getline(infile, sCurrentLine);
		line++;
	}
	return mat;
}
