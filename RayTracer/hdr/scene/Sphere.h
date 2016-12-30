/*
 * Sphere.h
 *
 *  Created on: 4-nov.-2016
 *      Author: arthu
 */

#ifndef SCENE_SPHERE_H_
#define SCENE_SPHERE_H_

#include <vector>

#include "GeoObj.h"

class Ray;
class Hit;

class Sphere : public GeoObj{
public:
	Sphere();
	virtual ~Sphere();

	void hit(Ray*, std::vector<Hit*>*);
	bool hitShadow(Ray*);
};

#endif /* SCENE_SPHERE_H_ */
