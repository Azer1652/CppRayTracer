/*
 * Cone.h
 *
 *  Created on: 4-jan.-2017
 *      Author: arthu
 */

#ifndef SCENE_CONE_H_
#define SCENE_CONE_H_

#include <vector>

#include "GeoObj.h"

class Ray;
class Hit;

class Cone : public GeoObj{
public:
	Cone();
	virtual ~Cone();

	void hit(Ray*, std::vector<Hit*>*);
	bool hitShadow(Ray*);

private:
	double* formNormal(double*);
};

#endif /* SCENE_CONE_H_ */
