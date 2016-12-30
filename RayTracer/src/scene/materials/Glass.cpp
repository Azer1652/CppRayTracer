/*
 * Glass.cpp
 *
 *  Created on: 1-nov.-2016
 *      Author: arthu
 */

#include "../../../hdr/scene/materials/Glass.h"

Glass::Glass() {
	this->transparency = .90;
	this->reflectivity = 0.05;
	this->refIndex = 1.46;
	this->specularExponent=1.0;
	this->specularFraction = 0;
	setAmbient(new double[3]{0,0,0});
	setDiffuse(new double[3]{0,0,0});
	setSpecular(new double[3]{0,0,0});
	setEmissive(new double[3]{0,0,0});

}

Glass::~Glass() {
	// TODO Auto-generated destructor stub
}

