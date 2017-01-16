/*
 * Cylinder.h
 *
 *  Created on: 6-jan.-2017
 *      Author: arthu
 */

#ifndef SCENE_CYLINDER_H_
#define SCENE_CYLINDER_H_

#include <vector>

#include "GeoObj.h"

class Ray;
class Hit;


class Cylinder : public GeoObj{
public:
	Cylinder();
	virtual ~Cylinder();

	void hit(Ray*, std::vector<Hit*>*);
	bool hitShadow(Ray*);

private:
	double* getNormal(int, bool);
};

#endif /* SCENE_CYLINDER_H_ */
