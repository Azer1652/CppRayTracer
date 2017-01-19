/*
 * Tracer.cpp
 *
 *  Created on: 2 Oct 2016
 *      Author: arthu
 */

#include "../hdr/Tracer.h"
#include "../hdr/RayTracer.h"
#include "../hdr/RayManager.h"
#include "../hdr/Ray.h"
#include "../hdr/Lib.h"
#include "../hdr/scene/Scene.h"
#include "../hdr/scene/Camera.h"
#include "../hdr/matrices/ViewMatrix.h"

#include <ctime>
#include <iostream>

/*
Tracer::Tracer() {
	// TODO Auto-generated constructor stub
}
*/

Tracer::Tracer(Scene* scn){
	if(numThreads==0){
		numThreads = 1;
	}
	this->scene = scn;

	x=0;
	y=0;

	divisions = WIDTH/(double)numThreads;
}

Tracer::~Tracer(){
}

void Tracer::start(int recursion, int blockSize, bool shadows, ViewMatrix* matrix){
	//start timer
	clock_t begin = clock();

	/*
	//Singlethreaded approach for simplification
	mans.push_back(new RayManager(this, recursion, blockSize, shadows, matrix));
	mans[0]->run();
	*/


	//threads
	int div = 0;
	int div2 = divisions;
	thread myThreads[numThreads];
	for(int g = 0; g<numThreads; g++)
	{
		RayManager* man = new RayManager(g, div, div2, this, recursion, blockSize, shadows, matrix);
		div += divisions;
		div2 += divisions;
		mans.push_back(man);

		myThreads[g] = thread([man]{man->run();});
	}

	for(int g = 0; g<numThreads; g++)
	{
		myThreads[g].join();
	}

	for(RayManager* man : mans)
		delete man;
	mans.clear();


	RayTracer::drawn = true;
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	if(RayTracer::debug)
		cout << "Elapsed time: " << elapsed_secs << " sec." << endl;
}

Scene* Tracer::getScene(){
	return scene;
}

double* Tracer::getCamLocation(){
	return scene->getCam()->getLoc();
}

double* Tracer::getCamLookAt(){
	return scene->getCam()->getLookAt();
}

double Tracer::getCamNear(){
	return scene->getCam()->getNear();
}

double Tracer::getCamFar(){
	return scene->getCam()->getFar();
}
double Tracer::getCamW(){
	return scene->getCam()->getW();
}

double Tracer::getCamH(){
	return scene->getCam()->getH();
}

double* Tracer::getCamN(){
	return scene->getCam()->getN();
}

double* Tracer::getCamU(){
	return scene->getCam()->getU();
}

double* Tracer::getCamV(){
	return scene->getCam()->getV();
}
