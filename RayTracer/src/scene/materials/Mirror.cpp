/*
 * Mirror.cpp
 *
 *  Created on: 1-nov.-2016
 *      Author: arthu
 */

#include "../../../hdr/scene/materials/Mirror.h"

Mirror::Mirror() {
	this->reflectivity = .99;
	this->specularExponent = 10;
	this->specularFraction = 0;
	setAmbient(new double[3]{0,0,0});
	setDiffuse(new double[3]{0,0,0});
	setSpecular(new double[3]{0,0,0});
	setEmissive(new double[3]{0,0,0});
}

Mirror::~Mirror() {
	// TODO Auto-generated destructor stub
}

