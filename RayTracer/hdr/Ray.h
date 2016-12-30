/*
 * Ray.h
 *
 *  Created on: 3 Oct 2016
 *      Author: arthu
 */

#ifndef RAY_H_
#define RAY_H_

#include <cmath>

class Tracer;
class GeoObj;
class Hit;

class Ray {
public:
	Ray();
	Ray(Tracer*, double*, double*);
	Ray(Tracer*);
	virtual ~Ray();

	void init();

	Tracer* getTracer();

	void run(double*);

	void setLocation(double, double, double);
	void setLocation(double*);
	void setDirection(double, double, double);
	void setDirection(double*);

	double* getDirection();
	double* getLocation();
	double* getColor();

	void setColor(double*);
	void setColor(double, double, double);

	void setRefIndex(double);
	void setMaxNumBounces(int);
	void setShadows(bool);

private:
	 double eps = 0.00000000000001;
	 double refEps = 0.0000000000000001;
	 double* location;
	 double* direction;
	 double* color;
	 int maxNumBounces = 0;
	 bool shadows = false;
	 double refIndex = 1;
	 Tracer* tracer;

	 bool isInShadow(Ray*);
	 double* setResult(Hit*);
	 void calculate(Hit*);
	 Hit* traceObjects();
};

#endif /* RAY_H_ */
