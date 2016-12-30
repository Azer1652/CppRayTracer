/*
 * Camera.cpp
 *
 *  Created on: 5 Oct 2016
 *      Author: arthu
 */

#include "../../hdr/scene/Camera.h"

#include "../../hdr/RayTracer.h"
#include "../../hdr/scene/GeoObj.h"
#include "../../hdr/Lib.h"

using namespace std;

Camera::Camera() {
	this->near1 = 0.1;
	this->viewAngle = 90;
	aspect = (double)WIDTH/(double)HEIGHT;
	H = (near1 * tan(PI_2 * viewAngle/(2.0 * 180)));
	W = (aspect * H);
	pos = new double[3]{0,0,0};
	lookAt = new double[3] {0,0,1};
	up = new double[3] {0,1,0};
	n = new double[3] {0,0,0};
	u = new double[3] {0,0,0};
	v = new double[3] {0,0,0};

	recalc();
}

Camera::~Camera() {
	delete[] pos;
	delete[] lookAt;
	delete[] up;
}

void Camera::recalc(){
	W = (aspect * H);
	setVariable(n,pos[0] - lookAt[0], pos[1] - lookAt[1], pos[2] - lookAt[2]);
	//this cross b
	//y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x
	//u.set(up.cross(n));
	setVariable(u,up[1]*n[2] - up[2]*n[1], up[2]*n[0] - up[0]*n[2], up[0]*n[1] - up[1]*n[0]);
	//v.set(n.cross(u));
	setVariable(v,n[1]*u[2] - n[2]*u[1], n[2]*u[0] - n[0]*u[2], n[0]*u[1] - n[1]*u[0]);
	Lib::normalize(u); Lib::normalize(v); Lib::normalize(n);
	//System.out.println("Break recalc");
}

void Camera::setVariable(double* d, double a, double b, double c){
	d[0] = a;
	d[1] = b;
	d[2] = c;
}

void Camera::setPos(double* pos){
	if(this->pos != NULL)
		delete this->pos;
	this->pos = pos;
	recalc();
}

void Camera::setLookAt(double* dir){
	if(this->lookAt != NULL)
		delete this->lookAt;
	this->lookAt = dir;
	recalc();
}

void Camera::setUp(double* up){
	if(this->up != NULL)
		delete this->up;
	this->up = up;
	recalc();
}

void Camera::setAspect(double asp){
	this->aspect = asp;
	recalc();
}

double* Camera::getLoc(){
	return pos;
}

double* Camera::getLookAt(){
	return lookAt;
}

double Camera::getNear(){
	return near1;
}

double Camera::getFar(){
	return far1;
}

double Camera::getW(){
	return W;
}

double Camera::getH(){
	return H;
}

double* Camera::getN(){
	return n;
}
double* Camera::getU(){
	return u;
}

double* Camera::getV(){
	return v;
}
