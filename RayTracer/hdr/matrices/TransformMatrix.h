/*
 * transformMatrix.h
 *
 *  Created on: 25 Oct 2016
 *      Author: arthu
 */

#ifndef TRANSFORMMATRIX_H_
#define TRANSFORMMATRIX_H_

#include <cstddef>

#include <gmtl/gmtl.h>

class TransformMatrix {
public:
	TransformMatrix();
	TransformMatrix(gmtl::Matrix44d*);
	virtual ~TransformMatrix();

	void generateMatrix();
	void deleteMatrix();

	double* getPArr();

	double** p;
};

#endif /* TRANSFORMMATRIX_H_ */
