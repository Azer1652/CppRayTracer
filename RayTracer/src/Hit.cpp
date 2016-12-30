/*
 * Hit.cpp
 *
 *  Created on: 3 Oct 2016
 *      Author: arthu
 */

#include "../hdr/Hit.h"
#include "../hdr/scene/GeoObj.h"

Hit::Hit() {
	// TODO Auto-generated constructor stub
	this->obj = NULL;
	normal = new double[3] {0,0,0};
	position = new double[3] {0,0,0};
	time = -1;
	entering = false;
}

Hit::Hit(Hit* h) {
	this->obj = h->obj;
	normal = new double[3] {h->getNormal()[0],h->getNormal()[1],h->getNormal()[2]};
	position = new double[3] {h->getPosition()[0],h->getPosition()[1],h->getPosition()[2]};
	time = h->getTime();
	entering = h->getEntering();
}

Hit::Hit(double time, GeoObj* obj , double* normal, double* position, bool entering){
	this->time=time;
	this->obj = obj;
	this->normal=normal;
	this->position=position;
	this->time = -1;
	this->entering = entering;
}

Hit::~Hit() {
	delete[] normal;
	delete[] position;
}

double* Hit::getNormal(){
	return normal;
}

void Hit::setNormal(double* normal) {
	if(this->normal != NULL)
		delete[] this->normal;
	this->normal = normal;
}

double* Hit::getPosition(){
	return position;
}

void Hit::setPosition(double* position) {
	if(this->position != NULL)
		delete[] this->position;
	this->position = position;
}

bool Hit::getEntering(){
	return entering;
}

void Hit::setEntering(bool entering) {
	this->entering = entering;
}

GeoObj* Hit::getObj(){
	return obj;
}

void Hit::setObj(GeoObj* obj){
	if(this->obj != NULL)
		delete this->obj;
	this->obj = obj;
}

double Hit::getTime(){
	return time;
}

void Hit::setTime(double time) {
	this->time = time;
}
