/*
 * ViewMatrix.cpp
 *
 *  Created on: 25 Oct 2016
 *      Author: arthu
 */

#include "../../hdr/matrices/ViewMatrix.h"

ViewMatrix::ViewMatrix() {
	generateMatrix(1920,1080,3);

}

ViewMatrix::~ViewMatrix() {
	deleteMatrix(1920,1080,3);
}

void ViewMatrix::generateMatrix(int width, int height, int depth){
	// Allocate memory
	p2DArray = new double**[height];
	for (int i = 0; i < height; ++i) {
	p2DArray[i] = new double*[width];

	for (int j = 0; j < width; ++j)
	  p2DArray[i][j] = new double[depth]{0,0,0};
	}
}

void ViewMatrix::deleteMatrix(int width, int height, int depth){
	// De-Allocate memory to prevent memory leak
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j){
		  delete [] p2DArray[i][j];
		}
		delete [] p2DArray[i];
	}
	delete [] p2DArray;
}
