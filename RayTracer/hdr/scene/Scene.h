/*
 * Scene.h
 *
 *  Created on: 4 Oct 2016
 *      Author: arthu
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <vector>
#include <string>

#include "../../hdr/Tracer.h"

class GeoObj;
class Light;
class Camera;
class Tracer;
class Parser;
class ViewMatrix;

using namespace std;

class Scene {
public:
	Scene(string, int, int);
	virtual ~Scene();

	void startTrace(ViewMatrix*);

	void setLocation(double*);
	void setLookAt(double*);
	void setUp(double*);

	void setCamAspect(double);

	double* getAmbient();
	vector<Light*>* getLights();
	vector<GeoObj*>* getObjects();

	double getRefIndex();

	Camera* getCam();

	Tracer* getTracer();

private:
	vector<GeoObj*> objects;
	vector<Light*> lights;
	Camera* cam;
	Parser* parser;
	double* ambient;

	int recursion = 2;
	int blockSize = 1;
	bool shadows = true;
	double refIndex = 1.000293;

	Tracer* tracer;
};

#endif /* SCENE_H_ */
