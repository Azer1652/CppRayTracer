/*
 * Scene.cpp
 *
 *  Created on: 4 Oct 2016
 *      Author: arthu
 */

#include "../../hdr/scene/Scene.h"

#include "../../hdr/scene/GeoObj.h"
#include "../../hdr/scene/Light.h"
#include "../../hdr/scene/Camera.h"
#include "../../hdr/scene/sdl/Parser.h"
#include "../../hdr/Tracer.h"

#include <stdio.h>

Scene::Scene(string fileName, int recursion, int blockSize){
	this->tracer = new Tracer(this);
	cam = new Camera();
	parser = new Parser(this, fileName);
	ambient = new double[3]{0.8,0.8,0.8};
	this->recursion=recursion;
	this->blockSize=blockSize;
	cout << "numObj: " << objects.size() << endl;
	cout << "numLights: " << lights.size() << endl;
	cout << "cam loc: " << cam->pos[0] << ", " << cam->pos[1] << ", " << cam->pos[2] << endl;
	cout << "cam lookAt: " << cam->lookAt[0] << ", " << cam->lookAt[1] << ", " << cam->lookAt[2] << endl;
}

Scene::~Scene() {
	// TODO Auto-generated destructor stub
	delete[] ambient;
	delete tracer;
	delete parser;
	for(Light* light : lights)
		delete light;
	lights.clear();
	for(GeoObj* obj : objects)
			delete obj;
	objects.clear();
}

void Scene::startTrace(ViewMatrix* matrix){
	tracer->start(recursion, blockSize, shadows, matrix);
}

double Scene::getRefIndex(){
	return refIndex;
}

Tracer* Scene::getTracer(){
	return tracer;
}

void Scene::setLocation(double* loc){
	this->cam->setPos(loc);
}

void Scene::setLookAt(double* dir){
	this->cam->setLookAt(dir);
}

void Scene::setUp(double* up){
	this->cam->setUp(up);
}

void Scene::setCamAspect(double asp){
	this->cam->setAspect(asp);
}

double* Scene::getAmbient(){
	return ambient;
}

vector<Light*>* Scene::getLights(){
	return &lights;
}

vector<GeoObj*>* Scene::getObjects(){
	return &objects;
}

Camera* Scene::getCam(){
	return cam;
}
