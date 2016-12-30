/*
 * Tracer.h
 *
 *  Created on: 2 Oct 2016
 *      Author: arthu
 */

#ifndef TRACER_H_
#define TRACER_H_

#include <vector>
#include <thread>

#include "../hdr/RayTracer.h"

class Scene;
class Ray;
class RayManager;
class Obj;
class ViewMatrix;

class Tracer {
public:
	Tracer(Scene*);
	void start(int, int, bool, ViewMatrix*);
	virtual ~Tracer();

	Scene* getScene();

	double* getCamLocation();
	double* getCamLookAt();
	std::vector<Obj*> getObjects();
	//implementation
	double getCamNear();
	double getCamFar();
	double getCamW();
	double getCamH();
	double* getCamN();
	double* getCamU();
	double* getCamV();

	int numThreads = 8;
private:

	Scene* scene;

	int x, y;
	std::vector<RayManager*> mans;
	int divisions;

	void setResult(int, double[]);

	void changeCheck(int);
};

#endif /* TRACER_H_ */
