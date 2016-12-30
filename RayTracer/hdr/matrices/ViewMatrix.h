/*
 * ViewMatrix.h
 *
 *  Created on: 25 Oct 2016
 *      Author: arthu
 */

#ifndef VIEWMATRIX_H_
#define VIEWMATRIX_H_

class ViewMatrix {
public:
	ViewMatrix();
	virtual ~ViewMatrix();

	double*** p2DArray;

	void generateMatrix(int, int, int);
	void deleteMatrix(int, int, int);
};

#endif /* VIEWMATRIX_H_ */
