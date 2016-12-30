/*
 * Lib.h
 *
 *  Created on: 5 Oct 2016
 *      Author: arthu
 */

#ifndef LIB_H_
#define LIB_H_

#include <cmath>

class Ray;
class TransformMatrix;

class Lib {
public:
	Lib();
	virtual ~Lib();

	static void normalize(double*);
	static void multiply(double* a, double* b);
	static void multiply(double* a, double b);
	static void add(double*, double*);
	static double* xfrmNormal(TransformMatrix*, double*);
	static void xfrmRay(Ray*, TransformMatrix*, Ray*);

	static double dot3D(double*, double*);
	static void dot3DArr(double*, double*, double*);
	static void dot3DArr(double*, double, double*);

	static bool generateInverse(double[16], double[16]);

	static void postMultiply4(TransformMatrix*, TransformMatrix*, TransformMatrix*);
};

#endif /* LIB_H_ */
