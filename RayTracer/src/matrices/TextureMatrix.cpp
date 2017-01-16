/*
 * TextureMatrix.cpp
 *
 *  Created on: 16-jan.-2017
 *      Author: arthu
 */

#include "../../hdr/matrices/TextureMatrix.h"

TextureMatrix::TextureMatrix(int width, int height) {
	this->width=width;
	this->height=height;
	generateMatrix(width,height,3);

}

TextureMatrix::~TextureMatrix() {
	deleteMatrix(width,height,3);
}

void TextureMatrix::generateMatrix(int width, int height, int depth){
	// Allocate memory
	array = new double**[width];
	for (int i = 0; i < width; ++i) {
	array[i] = new double*[height];

	for (int j = 0; j < height; ++j)
	  array[i][j] = new double[depth]{0,0,0};
	}
}

void TextureMatrix::deleteMatrix(int width, int height, int depth){
	// De-Allocate memory to prevent memory leak
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j){
		  delete [] array[i][j];
		}
		delete [] array[i];
	}
	delete [] array;
}
