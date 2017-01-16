/*
 * GeoObj.h
 *
 *  Created on: 3 Oct 2016
 *      Author: arthu
 */

#ifndef GEOOBJ_H_
#define GEOOBJ_H_

#include <cstddef>
#include <cmath>
#include <vector>

#include "Obj.h"

class Ray;
class Material;
class TransformMatrix;
class Hit;

namespace pi{
#define PI_2		3.14159265358979323846
}

enum{
	GEOOBJ,
	BOOLOBJ,
	BAR,
	SPHERE,
	CONE,
	CYLINDER
};

class GeoObj : public Obj{

public:
	GeoObj();
	virtual ~GeoObj();

	int type = GEOOBJ;

	double* dims;
	double* rot;

	//double color[3] = {0,0,0};
	TransformMatrix* transform;
	TransformMatrix* inverse;
	Material* mat = NULL;

	void setDims(double*);
	void setRot(double*);
	void setMat(Material*);
	Material* getMaterial();

	TransformMatrix* getTransform();
	TransformMatrix* getInverseTransform();

	void generateTransform();
	void generateInverse();

	virtual void hit(Ray*, std::vector<Hit*>*) = 0;
	virtual bool hitShadow(Ray*) = 0;

private:

};

#endif /* GEOOBJ_H_ */
