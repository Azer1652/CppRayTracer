/*
 * Lib.cpp
 *
 *  Created on: 5 Oct 2016
 *      Author: arthu
 */

#include "../hdr/Lib.h"
#include "../hdr/Ray.h"
#include "../hdr/matrices/TransformMatrix.h"

Lib::Lib() {
	// TODO Auto-generated constructor stub

}

Lib::~Lib() {
	// TODO Auto-generated destructor stub
}

void Lib::normalize(double* f){
	double sizeSq = f[0] * f[0] + f[1] * f[1] + f[2] * f[2];

	double scaleFactor = sqrt(sizeSq);
	f[0] /= scaleFactor; f[1] /= scaleFactor; f[2] /= scaleFactor;
}

void Lib::multiply(double* a, double* b){
	a[0] = a[0]*b[0];
	a[1] = a[1]*b[1];
	a[2] = a[2]*b[2];
}

void Lib::multiply(double* a, double b){
	a[0] = a[0]*b;
	a[1] = a[1]*b;
	a[2] = a[2]*b;
}

void Lib::add(double* a, double* b){
	a[0] += b[0];
	a[1] += b[1];
	a[2] += b[2];
}

double* Lib::xfrmNormal(TransformMatrix* aff, double* point){
	double* normal = new double[3] {0,0,0};
	normal[0] = (aff->p[0][0] * point[0] + aff->p[1][0] * point[1] + aff->p[2][0] * point[2]);
	normal[1] = (aff->p[0][1] * point[0] + aff->p[1][1] * point[1] + aff->p[2][1] * point[2]);
	normal[2] = (aff->p[0][2] * point[0] + aff->p[1][2] * point[1] + aff->p[2][2] * point[2]);
	return normal;
}

void Lib::xfrmRay(Ray* ray, TransformMatrix* transf, Ray* r){
	double x= (transf->p[0][0] * r->getLocation()[0] + transf->p[1][0] * r->getLocation()[1] + transf->p[2][0] * r->getLocation()[2] + transf->p[3][0]);
	double y= (transf->p[0][1] * r->getLocation()[0] + transf->p[1][1] * r->getLocation()[1] + transf->p[2][1] * r->getLocation()[2] + transf->p[3][1]);
	double z= (transf->p[0][2] * r->getLocation()[0] + transf->p[1][2] * r->getLocation()[1] + transf->p[2][2] * r->getLocation()[2] + transf->p[3][2]);

	ray->setLocation(x, y, z);

	double xx= (transf->p[0][0] * r->getDirection()[0] + transf->p[1][0] * r->getDirection()[1] + transf->p[2][0] * r->getDirection()[2]);
	double yy= (transf->p[0][1] * r->getDirection()[0] + transf->p[1][1] * r->getDirection()[1] + transf->p[2][1] * r->getDirection()[2]);
	double zz= (transf->p[0][2] * r->getDirection()[0] + transf->p[1][2] * r->getDirection()[1] + transf->p[2][2] * r->getDirection()[2]);
	ray->setDirection(xx, yy, zz);
}

double Lib::dot3D(double* a, double* b){
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void Lib::dot3DArr(double* a, double* b, double* result){
	result[0] = a[0]*b[0];
	result[1] = a[1]*b[1];
	result[2] = a[2]*b[2];
}

void Lib::dot3DArr(double* a, double b, double* result){
	result[0] = a[0]*b;
	result[1] = a[1]*b;
	result[2] = a[2]*b;
}

void Lib::postMultiply4(TransformMatrix* a, TransformMatrix* b, TransformMatrix* result){
	for(int i = 0; i<4; i++){
		for(int j=0; j<4; j++){
			double sum = 0;
			for(int k = 0; k<4; k++){
				sum += a->p[i][k]*b->p[k][j];
			}
			result->p[i][j] = sum;
		}
	}

}
