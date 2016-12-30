/*
 * Material.h
 *
 *  Created on: 5 Oct 2016
 *      Author: arthu
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <cstddef>

class Material {
public:
	Material();
	virtual ~Material();

	double* getEmissive();
	double* getAmbient();
	double* getDiffuse();
	double* getSpecular();
	double getSpecularExponent();

	double getReflectivity();
	double getTransparency();
	double getSpeedOfLight();
	double getSurfaceRoughness();
	double getRefIndex();

	void setReflectivity(double);
	void setTransparency(double);
	void setSpeedOfLight(double);

	void setSpecular(double*);
	void setAmbient(double*);
	void setDiffuse(double*);
	void setEmissive(double*);

	void setSpecularExponent(double);
	void setSpecularFraction(double);

	void setTexture(double);
	double getTexture();

protected:
	double* ambient;
	double* diffuse;
	double* specular;
	double* emissive;
	double specularExponent = 20.0;

	double reflectivity = 0;
	double transparency = 0;
	double speedOfLight = 20;
	double specularFraction = 0;
	double surfaceRoughness = 1;
	double texture = 0;
	double refIndex = 1;
};

#endif /* MATERIAL_H_ */
