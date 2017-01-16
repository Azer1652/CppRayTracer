/*
 * Ray.cpp
 *
 *  Created on: 3 Oct 2016
 *      Author: arthu
 */


#include "../hdr/Ray.h"
#include "../hdr/Tracer.h"
#include "../hdr/Hit.h"
#include "../hdr/scene/GeoObj.h"
#include "../hdr/scene/Scene.h"
#include "../hdr/scene/Bar.h"
#include "../hdr/scene/Sphere.h"
#include "../hdr/scene/Cone.h"
#include "../hdr/scene/Cylinder.h"
#include "../hdr/Lib.h"
#include "../hdr/scene/Light.h"
#include "../hdr/scene/materials/Material.h"
#include "../hdr/matrices/TextureMatrix.h"

#include <vector>
#include <algorithm>

using namespace std;

Ray::Ray() {
	this->tracer = NULL;
	this->direction = new double[3];
	this->location = new double[3];
	init();
}

Ray::Ray(Tracer* tracer, double* loc, double* dir) {
	this->tracer = tracer;
	this->direction = dir;
	this->location = loc;
	init();
}

Ray::Ray(Tracer* tracer) {
	this->tracer = tracer;
	double eps = 0.0000000000000001;
	refEps = 0.0000000000000001;
	this->direction = new double[3];
	this->location = new double[3];
	init();

}

Ray::~Ray() {
	// TODO Auto-generated destructor stub
	delete[] color;
	delete[] location;
	delete[] direction;
}

void Ray::init(){
	this->color = new double[3]{0,0,0};
}

Tracer* Ray::getTracer(){
	return tracer;
}

double* Ray::getLocation(){
	return location;
}

double* Ray::getDirection(){
	return direction;
}

double* Ray::getColor(){
	return color;
}

void Ray::run(double* result) {
	Hit* a = this->traceObjects();
	setResult(a);
	delete a;
	result[0] = color[0];
	result[1] = color[1];
	result[2] = color[2];
}

Hit* Ray::traceObjects(){
	Hit* bestHit = NULL;
	for(GeoObj* obj : *(tracer->getScene()->getObjects())){
		vector<Hit*> hitData;
		obj->hit(this, &hitData);

		if(!hitData.empty()){
			//SORT VECTOR
			std::sort(hitData.begin(), hitData.end(),
					[](const Hit* lhs, const Hit* rhs) {return lhs->time < rhs->time;});
			if(bestHit == NULL)
				bestHit = new Hit(hitData.front());
			else if(hitData.front()->getTime() < bestHit->getTime()){
					delete bestHit;
				bestHit = new Hit(hitData.front());
			}

			for(Hit* hit: hitData){
				delete hit;
			}
		}
	}
	return bestHit;
}

bool Ray::isInShadow(Ray* feeler){

	for(GeoObj* obj : *(tracer->getScene()->getObjects())){
		//TODO select best object and recalculate shadow hardness
		if(obj->hitShadow(feeler)){
			return true;
		}
	}
	return false;
}

double* Ray::setResult(Hit* a){
	if(a != NULL){
		//System.out.println("Object hit");
		calculate(a);
	}
	else{
		//Hit nothing
	}
	return color;
}

void Ray::calculate(Hit* a){
	//Emissive part
	GeoObj* b = a->getObj();
	double diffuse[3]{0,0,0};
	//Texture
	if(b->getMaterial()->textureFromFile){
		int y = (a->originalPosition[1]+1)*540;
		int x = (a->originalPosition[2]+1)*960;
		//Change all important colors with texture;
		diffuse[0] = b->getMaterial()->getTextureMatrix()->array[x][y][0];
		diffuse[1] = b->getMaterial()->getTextureMatrix()->array[x][y][1];
		diffuse[2] = b->getMaterial()->getTextureMatrix()->array[x][y][2];
	}else{
		diffuse[0] = b->getMaterial()->getDiffuse()[0];
		diffuse[1] = b->getMaterial()->getDiffuse()[1];
		diffuse[2] = b->getMaterial()->getDiffuse()[2];
	}
	if(b->getMaterial()->getTexture() == 1){
		int pos[3] = {a->getPosition()[0]*4, a->getPosition()[1]*4, a->getPosition()[2]*4};
		if(pos[0] % 2 == 0 && pos[2] % 2 == 0 && pos[1] %2 == 0){
			//Keep original color
		}
		else{
			Lib::dot3DArr(color, 0.4, color);
		}
	}
	double* emissive = b->getMaterial()->getEmissive();
	color[0] = emissive[0];
	color[1] = emissive[1];
	color[2] = emissive[2];
	//ambient part
	double* ambientClr = tracer->getScene()->getAmbient();
	Lib::multiply(ambientClr, b->getMaterial()->getAmbient());
	Lib::add(color, ambientClr);
	//Shadow Calculations
	//lights was itr
	vector<Light*>* lights = tracer->getScene()->getLights();
	double* normalN;
	normalN = Lib::xfrmNormal(b->getInverseTransform(), a->getNormal());
	Lib::normalize(normalN);
	double v[] {-this->direction[0], -this->direction[1], -this->direction[2]};
	Lib::normalize(v);
	for(Light* light : *lights){
		if(shadows){
			Ray* feeler = new Ray(tracer);
			//go back eps
			feeler->setLocation(a->getPosition()[0]-eps*this->direction[0],
							a->getPosition()[1]-eps*this->direction[1],
							a->getPosition()[2]-eps*this->direction[2]);
			feeler->setDirection(light->getPosition()[0]-a->getPosition()[0],
								light->getPosition()[1]-a->getPosition()[1],
								light->getPosition()[2]-a->getPosition()[2]);
			feeler->maxNumBounces = 0;
			if(isInShadow(feeler)){
				//System.out.println("In Shadow");
			} //shadow
			else{
				//System.out.println("No Shadow");
				//lightPos - hitPos
				double s[3] {light->getPosition()[0]-a->getPosition()[0],
								light->getPosition()[1]-a->getPosition()[1],
								light->getPosition()[2]-a->getPosition()[2]};
				Lib::normalize(s);
				double mDotS = s[0]*normalN[0] + s[1]*normalN[1] + s[2]*normalN[2];
				if(mDotS>0){
					double diffColor[3] {diffuse[0],diffuse[1],diffuse[2]};
					Lib::multiply(diffColor, light->getColor());
					Lib::multiply(diffColor, mDotS);
					Lib::add(color, diffColor);
				}
				double h[3]{s[0]+v[0], s[1]+v[1], s[2]+v[2]};
				Lib::normalize(h);
				double mDotH = h[0]*normalN[0] + h[1]*normalN[1] + h[2]*normalN[2];
				if(mDotH>0){
					double phong = pow(mDotH, b->getMaterial()->getSpecularExponent());
					double specColor[3] {b->getMaterial()->getSpecular()[0],b->getMaterial()->getSpecular()[1],b->getMaterial()->getSpecular()[2]};
					Lib::multiply(specColor, light->getColor());
					Lib::multiply(specColor, phong);
					Lib::add(color, specColor);
				}

			}
			delete feeler;
		}
		else{
			double s[3] {light->getPosition()[0]-a->getPosition()[0],
							light->getPosition()[1]-a->getPosition()[1],
							light->getPosition()[2]-a->getPosition()[2]};
			Lib::normalize(s);
			double mDotS = s[0]*normalN[0] + s[1]*normalN[1] + s[2]*normalN[2];
			if(mDotS>0){
				double diffColor[3] {diffuse[0],diffuse[1],diffuse[2]};
				Lib::multiply(diffColor, light->getColor());
				Lib::multiply(diffColor, mDotS);
				Lib::add(color, diffColor);
			}
			double h[3]{s[0]+v[0], s[1]+v[1], s[2]+v[2]};
			Lib::normalize(h);
			double mDotH = h[0]*normalN[0] + h[1]*normalN[1] + h[2]*normalN[2];
			if(mDotH>0){
				double phong = pow(mDotH, b->getMaterial()->getSpecularExponent());
				double specColor[3] {b->getMaterial()->getSpecular()[0],b->getMaterial()->getSpecular()[1],b->getMaterial()->getSpecular()[2]};
				Lib::multiply(specColor, light->getColor());
				Lib::multiply(specColor, phong);
				Lib::add(color, specColor);
			}
		}
	}

	//allowed to bounce again?
	//TODO cant combine reflection and refraction 100% correct a.t.m. (reflective index doesn't support it)
	if(maxNumBounces > 0){
		//reflected part
		if(b->getMaterial()->getReflectivity() > 0.01){
			//Reflected ray
			//Get normalized normal
			double* normal;
			normal = Lib::xfrmNormal(b->getTransform(), a->getNormal());
			//calculate new ray direction
			Ray* reflectedRay = new Ray(tracer);
			reflectedRay->setLocation(a->getPosition()[0]-refEps*this->getDirection()[0],
									a->getPosition()[1]-refEps*this->getDirection()[1],
									a->getPosition()[2]-refEps*this->getDirection()[2]);
			reflectedRay->setDirection(this->direction[0]-2*(Lib::dot3D(this->direction,normal))*normal[0],
									this->direction[1]-2*(Lib::dot3D(this->direction,normal))*normal[1],
									this->direction[2]-2*(Lib::dot3D(this->direction,normal))*normal[2]);
			reflectedRay->setMaxNumBounces(maxNumBounces - 1);
			double* reflectedColor = new double[3] {0,0,0};
			reflectedRay->run(reflectedColor);
			double* temp = new double[3] {0,0,0};
			Lib::dot3DArr(reflectedColor, b->getMaterial()->getReflectivity(), temp);
			Lib::add(color, temp);
			delete[] normal;
			delete[] reflectedColor;
			delete[] temp;
			delete reflectedRay;
		}

		//transparent part
		if(b->getMaterial()->getTransparency() > 0.01){
			//Refracted ray
			//Snell's law
			//TODO total internal reflection
			double n = 0;

			//Get normalized normal
			double* normal;
			normal = Lib::xfrmNormal(b->getTransform(), a->getNormal());
			//calculate new ray direction
			Ray* refractedRay = new Ray(tracer);
			if(a->getEntering()){
				n = this->refIndex/b->getMaterial()->getRefIndex();
				refractedRay->setRefIndex(b->getMaterial()->getRefIndex());
			}
			else{
				n = b->getMaterial()->getRefIndex()/this->refIndex;
				refractedRay->setRefIndex(tracer->getScene()->getRefIndex());
			}
			double* dir = new double[3]{this->direction[0],this->direction[1],this->direction[2]};
			Lib::normalize(dir);
			if(1-(pow(n, 2))*(1-(pow(Lib::dot3D(normal, dir),2))) >= 0){
				double c2 = sqrt(1-(pow(n, 2))*(1-(pow(Lib::dot3D(normal, dir),2))));
				//go back
				refractedRay->setLocation(a->getPosition()[0]+refEps*this->getDirection()[0],
										a->getPosition()[1]+refEps*this->getDirection()[1],
										a->getPosition()[2]+refEps*this->getDirection()[2]);
				refractedRay->setDirection(n*dir[0]+(n*(Lib::dot3D(normal, dir))-c2)*normal[0],
										n*dir[1]+(n*(Lib::dot3D(normal, dir))-c2)*normal[1],
										n*dir[2]+(n*(Lib::dot3D(normal, dir))-c2)*normal[2]);
				refractedRay->setMaxNumBounces(maxNumBounces - 1);
				double* refractedColor = new double[3] {0,0,0};
				refractedRay->run(refractedColor);
				double* temp = new double[3] {0,0,0};
				Lib::dot3DArr(refractedColor, b->getMaterial()->getTransparency(), temp);
				Lib::add(color, temp);

				delete[] temp;
				delete[] refractedColor;

			}
			delete[] normal;
			delete refractedRay;
			//else do nothing
		}
	}

	if(color[0] > 1)
		color[0] = 1;
	if(color[1] > 1)
		color[1] = 1;
	if(color[2] > 1)
		color[2] = 1;
	delete[] normalN;
	//return color;
}

void Ray::setLocation(double x, double y, double z){
	this->location[0] = x;
	this->location[1] = y;
	this->location[2] = z;
}

void Ray::setLocation(double* xyz){
	delete[] this->location;
	this->location = xyz;
}

void Ray::setDirection(double x, double y, double z){
	this->direction[0] = x;
	this->direction[1] = y;
	this->direction[2] = z;
}

void Ray::setDirection(double* xyz){
	delete[] this->direction;
	this->direction = xyz;
}

void Ray::setColor(double*){
	this->color = color;
}

void Ray::setColor(double r, double g, double b){
	this->color[0] = r;
	this->color[1] = g;
	this->color[2] = b;
}

void Ray::setRefIndex(double ref){
	this->refIndex = ref;
}

void Ray::setMaxNumBounces(int max){
	this->maxNumBounces = max;
}

void Ray::setShadows(bool shadows){
	this->shadows = shadows;
}
