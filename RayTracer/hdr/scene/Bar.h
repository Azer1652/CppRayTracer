/*
 * Bar.h
 *
 *  Created on: 5 Oct 2016
 *      Author: arthu
 */

#ifndef BAR_H_
#define BAR_H_

#include <vector>

#include "GeoObj.h"

class Ray;
class Hit;

class Bar : public GeoObj{
public:
	Bar();
	virtual ~Bar();

	void hit(Ray*, std::vector<Hit*>*);
	bool hitShadow(Ray*);

private:
	double* getNormal(int);

};

#endif /* BAR_H_ */

