/*
 * Obj.cpp
 *
 *  Created on: 14 Oct 2016
 *      Author: arthu
 */

#include "../../hdr/scene/Obj.h"

Obj::Obj() {
	// TODO Auto-generated constructor stub
	this->pos = new double[3]{0,0,0};
	this->color = new double[3]{0,0,0};
}

Obj::~Obj() {
	// TODO Auto-generated destructor stub
	delete[] pos;
	delete[] color;
}

void Obj::setPosition(double* pos){
	this->pos[0] = pos[0];
	this->pos[1] = pos[1];
	this->pos[2] = pos[2];
}

void Obj::setColor(double* color){
	this->color[0] = color[0];
	this->color[1] = color[1];
	this->color[2] = color[2];
}

