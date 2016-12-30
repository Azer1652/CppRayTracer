/*
 * RayManager.h
 *
 *  Created on: 3 Oct 2016
 *      Author: arthu
 */

#ifndef RAYMANAGER_H_
#define RAYMANAGER_H_

class Tracer;
class ViewMatrix;
#include "RayTracer.h"

class RayManager {
public:
	//singlethreaded constuctor
	RayManager(Tracer*, int, int, bool, ViewMatrix*);
	//multithreaded constructors
	RayManager();
	RayManager(int, int, int, Tracer*, int, int, bool, ViewMatrix*);
	virtual ~RayManager();
	void run();
private:
	int div, div2;
	Tracer* tracer;
	ViewMatrix* matrix;
	int recursion, blockSize;
	bool shadows;
	int id;
};

#endif /* RAYMANAGER_H_ */
