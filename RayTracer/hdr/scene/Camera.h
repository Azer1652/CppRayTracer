/*
 * Camera.h
 *
 *  Created on: 5 Oct 2016
 *      Author: arthu
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <math.h>
#define PI_2		3.14159265358979323846

class GeoObj;

class Camera{
public:
	Camera();
	virtual ~Camera();

	double near1;
	double far1;
	double H;
	double W;
	double viewAngle;
	double aspect;
	double* n;
	double* u;
	double* v;

	double* pos;
	double* lookAt;
	double* up;

	void recalc();
	void setPos(double*);
	void setLookAt(double*);
	void setUp(double*);
	void setAspect(double);

	double* getLoc();
	double* getLookAt();

	double getNear();
	double getFar();
	double getW();
	double getH();
	double* getN();
	double* getU();
	double* getV();

private:
	void setVariable(double*, double, double, double);

};

#endif /* CAMERA_H_ */
