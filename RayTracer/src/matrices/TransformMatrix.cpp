/*
 * transformMatrix.cpp
 *
 *  Created on: 25 Oct 2016
 *      Author: arthu
 */

#include "../../hdr/matrices/TransformMatrix.h"

TransformMatrix::TransformMatrix() {
	generateMatrix();
}

TransformMatrix::TransformMatrix(gmtl::Matrix44d* mat) {
	generateMatrix();
	const double* temp = mat->getData();
	int i=0;
	for(int j=0; j<4; j++){
		for(int k=0; k<4; k++){
			p[j][k] = temp[i];
			i++;
		}
	}
}

TransformMatrix::~TransformMatrix() {
	deleteMatrix();
}

double* TransformMatrix::getPArr(){
	double* x = new double[16];
	int i=0;
	for(int j=0; j<4; j++){
		for(int k=0; k<4; k++){
			x[i]=p[j][k];
			i++;
		}
	}
	return x;
}

void TransformMatrix::generateMatrix(){
	p = new double*[4]; // dynamic `array (size 4) of pointers to int`

	for (int i = 0; i < 4; ++i) {
	  p[i] = new double[4]{0,0,0,0};
	  // each i-th pointer is now pointing to dynamic array (size 10) of actual int values
	}
}

void TransformMatrix::deleteMatrix(){
	for (int i = 0; i < 4; ++i)
		delete [] p[i];
	  delete [] p;
}
