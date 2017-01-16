/*
 * Material.cpp
 *
 *  Created on: 5 Oct 2016
 *      Author: arthu
 */

#include "../../../hdr/scene/materials/Material.h"
#include "../../../hdr/matrices/TextureMatrix.h"

#include <stdio.h>
#include <string>

using namespace std;

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

TextureMatrix* Material::getTextureMatrix(){
	return textureMatrix;
}

void Material::readBMP(const char* filename){
	string s = "./files/textures/";
	FILE* f = fopen(s.append(filename).c_str(), "rb");
	if (f!=NULL)
	{
		textureFromFile=true;
		unsigned char info[54];
		fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

		// extract image height and width from header
		textureWidth = *(int*)&info[18];
		textureHeight = *(int*)&info[22];

		int row_padded = (textureWidth*3 + 3) & (~3);
		unsigned char* data = new unsigned char[row_padded];
		unsigned char tmp;

		textureMatrix = new TextureMatrix(textureWidth, textureHeight);
		int realJ = 0;
		for(int i = 0; i < textureHeight; i++){
			fread(data, sizeof(unsigned char), row_padded, f);
			for(int j = 0; j < textureWidth*3; j += 3){
				delete[] textureMatrix->array[realJ][i];
				textureMatrix->array[realJ][i] = new double[3]{(int)data[j+2], (int)data[j+1], (int)data[j]};
				textureMatrix->array[realJ][i][0] /= 255.0;
				textureMatrix->array[realJ][i][1] /= 255.0;
				textureMatrix->array[realJ][i][2] /= 255.0;
				realJ++;
			}
			realJ=0;
		}

		fclose(f);
		delete[] data;
	}

}
