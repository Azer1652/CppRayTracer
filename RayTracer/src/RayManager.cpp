/*
 * RayManager.cpp
 *
 *  Created on: 3 Oct 2016
 *      Author: arthu
 */

#include "../hdr/RayManager.h"
#include "../hdr/Tracer.h"
#include "../hdr/RayTracer.h"
#include "../hdr/Ray.h"
#include "../hdr/Lib.h"
#include "../hdr/matrices/ViewMatrix.h"

#include <iostream>

RayManager::RayManager(Tracer* tracer, int recursion, int blockSize, bool shadows, ViewMatrix* matrix) {
	// TODO Auto-generated constructor stub
	this->div = 0;
	this->div2 = WIDTH;
	this->tracer = tracer;
	this->id=1;
	this->recursion = recursion;
	this->shadows = shadows;
	this->blockSize = blockSize;
	this->matrix = matrix;
}

RayManager::RayManager(){
	div = 0;
	div2 = 0;
	tracer = NULL;
	id = 0;
	recursion = 4;
	shadows = true;
	blockSize = 1;
	matrix = NULL;
}

RayManager::RayManager(int id, int div, int div2, Tracer* tracer, int recursion, int blockSize, bool shadows, ViewMatrix* matrix){
	this->div = div;
	this->div2 = div2;
	this->tracer = tracer;
	this->id=id;
	this->recursion = recursion;
	this->shadows = shadows;
	this->blockSize = blockSize;
	this->matrix = matrix;
}

RayManager::~RayManager() {
	// TODO Auto-generated destructor stub
}

void RayManager::run(){
	//Singlethreaded approach
	int i = div, j = 0;
	//brag();
	double blockWidth  = 2 * tracer->getCamW() / WIDTH;
	double blockHeight = 2 * tracer->getCamH() / HEIGHT;
	Ray* ray = new Ray(tracer);
	//divide screen in sectors
	//Calculate result for each section

	if(this->id == 0){
		//std::cout << "Break" << std::endl;
	}
	while(i<div2){
		while(j<HEIGHT){
			if(i == 1 && j == 400)
				std::cout << "Break;" << std::endl;
			double x = -tracer->getCamW() + (i) * blockWidth;
			double y = -tracer->getCamH() + (j) * blockHeight;
			//IMPROVE Possible speedup is function result saved
			ray->setColor(0, 0, 0);
			//Ray start direction
			ray->setDirection(-tracer->getCamNear() * tracer->getCamN()[0] + x * tracer->getCamU()[0] + y * tracer->getCamV()[0],
					   -tracer->getCamNear() * tracer->getCamN()[1] + x * tracer->getCamU()[1] + y * tracer->getCamV()[1],
					   -tracer->getCamNear() * tracer->getCamN()[2] + x * tracer->getCamU()[2] + y * tracer->getCamV()[2]);
			//start location = camera
			ray->setLocation(tracer->getCamLocation()[0], tracer->getCamLocation()[1], tracer->getCamLocation()[2]);
			ray->setRefIndex(1.000293);
			ray->setMaxNumBounces(recursion);
			ray->setShadows(shadows);

			//Lib::normalize(ray->getDirection());

			ray->run(matrix->p2DArray[i][j]);
			//Fill rectangle
			for(int k = 0; k < blockSize; k++){
				for(int l = 0; l < blockSize; l++){
					matrix->p2DArray[i+l][j+k][0] = matrix->p2DArray[i][j][0];
					matrix->p2DArray[i+l][j+k][1] = matrix->p2DArray[i][j][1];
					matrix->p2DArray[i+l][j+k][2] = matrix->p2DArray[i][j][2];
				}
			}
			j+=blockSize;
		}
		j = 0;
		std::cout << "Process: " << i << " out of " << div2 << "." << std::endl;
		i+=blockSize;
	}

	delete ray;
}
