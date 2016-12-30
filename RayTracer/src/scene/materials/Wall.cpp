/*
 * Wall.cpp
 *
 *  Created on: 1-nov.-2016
 *      Author: arthu
 */

#include "../../../hdr/scene/materials/Wall.h"

Wall::Wall() {
	this->reflectivity = 0;
	this->specularExponent = 5;
	this->specularFraction = 0;
	setAmbient(new double[3]{0.1,0.1,0.1});
	setDiffuse(new double[3]{0.5,0.5,0.5});
	setSpecular(new double[3]{0.1,.1,.1});
	setEmissive(new double[3]{0,0,0});
}

Wall::~Wall() {
	// TODO Auto-generated destructor stub
}

