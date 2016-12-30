/*
 * GeoObj.cpp
 *
 *  Created on: 3 Oct 2016
 *      Author: arthu
 */

#include "../../hdr/scene/GeoObj.h"

#include "../../hdr/scene/materials/Material.h"
#include "../../hdr/Ray.h"
#include "../../hdr/Lib.h"
#include "../../hdr/matrices/TransformMatrix.h"
#include <gmtl/gmtl.h>

#include <iostream>

using namespace std;

GeoObj::GeoObj() {
	transform = new TransformMatrix();
	inverse = NULL;
	dims = new double[3]{0,0,0};
	rot = new double[3]{0,0,0};
}

GeoObj::~GeoObj() {
	delete transform;
	delete inverse;
	delete mat;
	delete[] rot;
	delete[] dims;

}

void GeoObj::setDims(double* dims){
	if(this->dims != NULL)
		delete this->dims;
	this->dims = dims;
}

void GeoObj::setRot(double* rot){
	if(this->rot != NULL)
		delete this->rot;
	this->rot = rot;
}

void GeoObj::setMat(Material* mat){
	if(this->mat != NULL)
		delete this->mat;
	this->mat = mat;
}

Material* GeoObj::getMaterial(){
	return mat;
}

TransformMatrix* GeoObj::getTransform(){
	return transform;
}

TransformMatrix* GeoObj::getInverseTransform(){
	return inverse;
}

void GeoObj::generateTransform(){
	double* rot = this->rot;
	rot[0] = (rot[0]*PI_2)/180;
	rot[1] = (rot[1]*PI_2)/180;
	rot[2] = (rot[2]*PI_2)/180;

	TransformMatrix* result = new TransformMatrix();

	result->p[0][0] = this->dims[0];
	result->p[1][1] = this->dims[1];
	result->p[2][2] = this->dims[2];
	result->p[3][0] = this->pos[0];
	result->p[3][1] = this->pos[1];
	result->p[3][2] = this->pos[2];
	result->p[3][3] = 1;

	TransformMatrix* a = new TransformMatrix();

	a->p[0][0] = 1;
	a->p[1][1] = cos(rot[0]);
	a->p[2][1] = -sin(rot[0]);
	a->p[1][2] = sin(rot[0]);
	a->p[2][2] = cos(rot[0]);
	a->p[3][3] = 1;

	TransformMatrix* b = new TransformMatrix();

	b->p[0][0] = cos(rot[1]);
	b->p[2][0] = sin(rot[1]);
	b->p[1][1] = 1;
	b->p[0][2] = -sin(rot[1]);
	b->p[2][2] = cos(rot[1]);
	b->p[3][3] = 1;

	TransformMatrix* c = new TransformMatrix();

	c->p[0][0] = cos(rot[2]);
	c->p[1][0] = -sin(rot[2]);
	c->p[0][1] = sin(rot[2]);
	c->p[1][1] = cos(rot[2]);
	c->p[2][2] = 1;
	c->p[3][3] = 1;

	TransformMatrix* rotatezy = new TransformMatrix();
	TransformMatrix* rotate = new TransformMatrix();
	Lib::postMultiply4(b,c,rotatezy);
	Lib::postMultiply4(a,rotatezy, rotate);

	delete rotatezy;

	Lib::postMultiply4(rotate, result, this->transform);
	delete result;
	delete a;
	delete b;
	delete c;
	delete rotate;
}

void GeoObj::generateInverse(){
	gmtl::Matrix44d test_matrix;
	double* temp = transform->getPArr();
	test_matrix.set(temp);
	gmtl::Matrix44d test_inverse = gmtl::invert(test_matrix);
	delete temp;
	temp = NULL;
	inverse = new TransformMatrix(&test_inverse);

	//Lib::generateInverse(transform->getPArr(),inverse->getPArr());
}
