/*
 * Material.cpp
 *
 *  Created on: 5 Oct 2016
 *      Author: arthu
 */

#include "../../../hdr/scene/materials/Material.h"

Material::Material() {
	ambient = new double[3]{0,0,0};
	diffuse = new double[3]{0,0,0};
	emissive = new double[3]{0,0,0};
	specular = new double[3]{0,0,0};

}

Material::~Material() {
	delete[] ambient;
	delete[] diffuse;
	delete[] specular;
	delete[] emissive;
}

double* Material::getEmissive(){
	return emissive;
}

double* Material::getAmbient(){
	return ambient;
}

double* Material::getDiffuse(){
	return diffuse;
}

double* Material::getSpecular(){
	return specular;
}

double Material::getSpecularExponent(){
	return specularExponent;
}

double Material::getReflectivity(){
	return reflectivity;
}

double Material::getTransparency(){
	return transparency;
}

double Material::getRefIndex(){
	return refIndex;
}

void Material::setReflectivity(double x){
	this->reflectivity = x;
}

void Material::setTransparency(double x){
	this->transparency = x;
}

void Material::setSpeedOfLight(double x){
	this->speedOfLight = x;
}

void Material::setSpecular(double* x){
	if(this->specular != NULL)
		delete this->specular;
	this->specular = x;
}

void Material::setAmbient(double* x){
	if(this->ambient != NULL)
		delete this->ambient;
	this->ambient = x;
}

void Material::setDiffuse(double* x){
	if(this->diffuse != NULL)
		delete this->diffuse;
	this->diffuse = x;
}

void Material::setEmissive(double* x){
	if(this->emissive != NULL)
		delete this->emissive;
	this->emissive = x;
}

void Material::setSpecularExponent(double x){
	this->specularExponent = x;
}

void Material::setSpecularFraction(double x){
	this->specularFraction = x;
}

void Material::setTexture(double x){
	this->texture = x;
}

double Material::getTexture(){
	return texture;
}
