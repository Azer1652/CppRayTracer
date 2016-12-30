/*
 * Hit.h
 *
 *  Created on: 3 Oct 2016
 *      Author: arthu
 */

#ifndef HIT_H_
#define HIT_H_

#include <cstddef>

class GeoObj;

class Hit{
public:
	Hit();
	Hit(Hit*);
	Hit(double, GeoObj*, double*, double*, bool);
	virtual ~Hit();

	GeoObj* obj = NULL;

	bool getEntering();
	void setEntering(bool);

	void setNormal(double*);
	double* getNormal();

	void setPosition(double*);
	double* getPosition();

	void setTime(double);
	double getTime();

	GeoObj* getObj();
	void setObj(GeoObj*);

	double time;
private:
	double* normal;
	double* position;
	bool entering;
};

#endif /* HIT_H_ */
