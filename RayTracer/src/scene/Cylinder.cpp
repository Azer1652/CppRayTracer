/*
 * Cylinder.cpp
 *
 *  Created on: 6-jan.-2017
 *      Author: arthu
 */

#include "../../hdr/scene/Cylinder.h"

#include "../../hdr/Lib.h"
#include "../../hdr/Ray.h"
#include "../../hdr/Hit.h"

#include <cmath>

using namespace std;

Cylinder::Cylinder() {
	this->type = CYLINDER;

}

Cylinder::~Cylinder() {
	// TODO Auto-generated destructor stub
}

void Cylinder::hit(Ray* r, vector<Hit*>* hitData){
	//returns closest hitpoint for given ray
	Ray* ray = new Ray();
	Lib::xfrmRay(ray, this->getInverseTransform(), r);
	//returns closest hitpoint for given ray
	double A, B, C, discrim, disc_root, t1 = 0, t2 = 0;

	double* a = ray->getDirection();
	//System.out.println(a[0] + " " + a[1] + " " + a[2]);
	double* b = ray->getLocation();

	A = a[0] * a[0] + a[1] * a[1];
	//Divide by zero check
	if (A != 0){
		//B = dot3D(genRay.start, genRay.dir);
		B =2*( b[0] * a[0] + b[1] * a[1]);
		//C = dot3D(genRay.start, genRay.start) - 1.0;
		C = (b[0] * b[0] + b[1] * b[1] - 1);
		discrim = B * B -4* A * C;

		if(discrim < 0.0)
		{
			//System.out.println("Missed Ray");
			delete ray;
			return;   // ray missed return -1;
		}

		//System.out.println("Hit Ray" + ray.getCoordinates()[0] + " " + ray.getCoordinates()[1]);

		disc_root = sqrt(discrim);
		t1 = (-B - disc_root)/(2*A);
		t2 = (-B + disc_root)/(2*A);
		//System.out.println("t1: " + t1 + ", t2: " + t2);


		//bottom cap
		//z2 > min and z1 < min or reversed
		if((b[2] + t2*a[2] > 0 && b[2] + t1*a[2] < 0) || (b[2] + t1*a[2] > 0 && b[2] + t2*a[2] < 0)){
			double t3 = (0 - b[2])/a[2];
			if(t3 > 0){
				Hit* hit = new Hit();
				hit->setPosition(new double[3]{r->getLocation()[0]+r->getDirection()[0]*t3,
											r->getLocation()[1]+r->getDirection()[1]*t3,
											r->getLocation()[2]+r->getDirection()[2]*t3});
				hit->setTime(t3);
				hit->setObj(this);
				//ENTERING + NORMAL
				if(t2>t1){
					if(ray->getLocation()[2]+ray->getDirection()[2]*t2 < 0){
						hit->setEntering(false);
						hit->setNormal(new double[3]{0,0,1});
					}else{
						hit->setEntering(true);
						hit->setNormal(new double[3]{0,0,-1});
					}
				}else{
					if(ray->getLocation()[2]+ray->getDirection()[2]*t1 < 0){
						hit->setEntering(false);
						hit->setNormal(new double[3]{0,0,1});
					}else{
						hit->setEntering(true);
						hit->setNormal(new double[3]{0,0,-1});
					}
				}

				//System.out.println("normal: " + hit.getNormal()[0] + " " + hit.getNormal()[1] + " " + hit.getNormal()[2]);
				//extra hitData
				hitData->push_back(hit);
			}
		}

		//top cap
		//z2 > min een z1 < min or reversed
		if((b[2] + t2*a[2] > 1 && b[2] + t1*a[2] < 1) || (b[2] + t1*a[2] > 1 && b[2] + t2*a[2] < 1)){
			double t3 = (1 - b[2])/a[2];
			if(t3 > 0){
				Hit* hit = new Hit();
				hit->setPosition(new double[3]{r->getLocation()[0]+r->getDirection()[0]*t3,
											r->getLocation()[1]+r->getDirection()[1]*t3,
											r->getLocation()[2]+r->getDirection()[2]*t3});
				hit->setTime(t3);
				hit->setObj(this);
				//ENTERING + NORMAL
				if(t2>t1){
					if(ray->getLocation()[2]+ray->getDirection()[2]*t2 > 1){
						hit->setEntering(false);
						hit->setNormal(new double[3]{0,0,-1});
					}else{
						hit->setEntering(true);
						hit->setNormal(new double[3]{0,0,1});
					}
				}else{
					if(ray->getLocation()[2]+ray->getDirection()[2]*t1 > 1){
						hit->setEntering(false);
						hit->setNormal(new double[3]{0,0,-1});
					}else{
						hit->setEntering(true);
						hit->setNormal(new double[3]{0,0,1});
					}
				}

				//System.out.println("normal: " + hit.getNormal()[0] + " " + hit.getNormal()[1] + " " + hit.getNormal()[2]);
				//extra hitData
				hitData->push_back(hit);
			}
		}

		if(t2 > 0.00001){
			//System.out.println("Positive Hit");
			if(b[2] + t2*a[2] > 0 && b[2] + t2*a[2] < 1){
				Hit* hit = new Hit();
				hit->setPosition(new double[3]{r->getLocation()[0]+r->getDirection()[0]*t2,
											r->getLocation()[1]+r->getDirection()[1]*t2,
											r->getLocation()[2]+r->getDirection()[2]*t2});
				hit->setTime(t2);
				hit->setObj(this);
				if(t2<t1 && t1 >0){
					hit->setEntering(true);
					hit->setNormal(new double[3]{ray->getLocation()[0]+ray->getDirection()[0]*t2,
												ray->getLocation()[1]+ray->getDirection()[1]*t2,
												0});
				}else{
					hit->setEntering(false);
					hit->setNormal(new double[3]{-(ray->getLocation()[0]+ray->getDirection()[0]*t2),
												-(ray->getLocation()[1]+ray->getDirection()[1]*t2),
												0});
				}

				//System.out.println("normal: " + hit.getNormal()[0] + " " + hit.getNormal()[1] + " " + hit.getNormal()[2]);
				//extra hitData
				hitData->push_back(hit);
			}
		}

		if(t1 > 0.00001){
			//System.out.println("Positive Hit");
			if(b[2] + t1*a[2] > 0 && b[2] + t1*a[2] < 1){
				Hit* hit = new Hit();
				hit->setPosition(new double[3]{r->getLocation()[0]+r->getDirection()[0]*t1,
											r->getLocation()[1]+r->getDirection()[1]*t1,
											r->getLocation()[2]+r->getDirection()[2]*t1});
				hit->setTime(t1);
				hit->setObj(this);
				if(t1<t2 && t2 >0){
					hit->setEntering(true);
					hit->setNormal(new double[3]{ray->getLocation()[0]+ray->getDirection()[0]*t1,
												ray->getLocation()[1]+ray->getDirection()[1]*t1,
												0});
				}
				else{
					hit->setEntering(false);
					hit->setNormal(new double[3]{-(ray->getLocation()[0]+ray->getDirection()[0]*t1),
												-(ray->getLocation()[1]+ray->getDirection()[1]*t1),
												0});
				}

				//System.out.println("normal: " + hit.getNormal()[0] + " " + hit.getNormal()[1] + " " + hit.getNormal()[2]);
				//extra hitData
				hitData->push_back(hit);
			}
		}
		delete ray;
	}
}

bool Cylinder::hitShadow(Ray* r){
	//returns closest hitpoint for given ray
	Ray* ray = new Ray();
	Lib::xfrmRay(ray, this->getInverseTransform(), r);
	//returns closest hitpoint for given ray
	double A, B, C, discrim, disc_root, t1 = 0, t2 = 0;

	double* a = ray->getDirection();
	//System.out.println(a[0] + " " + a[1] + " " + a[2]);
	double* b = ray->getLocation();

	A = a[0] * a[0] + a[1] * a[1];
	//Divide by zero check
	if (A != 0){
		//B = dot3D(genRay.start, genRay.dir);
		B =2*( b[0] * a[0] + b[1] * a[1]);
		//C = dot3D(genRay.start, genRay.start) - 1.0;
		C = (b[0] * b[0] + b[1] * b[1] - 1);
		discrim = B * B - 4* A * C;

		if(discrim < 0.0)
		{
			//System.out.println("Missed Ray");
			delete ray;
			return false;   // ray missed return -1;
		}

		//System.out.println("Hit Ray" + ray.getCoordinates()[0] + " " + ray.getCoordinates()[1]);

		disc_root = sqrt(discrim);
		t1 = (-B - disc_root)/(2*A);
		t2 = (-B + disc_root)/(2*A);
		//System.out.println("t1: " + t1 + ", t2: " + t2);

		//bottom cap
		//z2 > min een z1 < min or reversed
		if((b[2] + t2*a[2] > 0 && b[2] + t1*a[2] < 0) || (b[2] + t1*a[2] > 0 && b[2] + t2*a[2] < 0)){
			double t3 = (0 - b[2])/a[2];
			if(t3 >0 && t3 < 1)
				return true;
		}

		//top cap
		//z2 > min een z1 < min or reversed
		if((b[2] + t2*a[2] > 1 && b[2] + t1*a[2] < 1) || (b[2] + t1*a[2] > 1 && b[2] + t2*a[2] < 1)){
			double t3 = (1 - b[2])/a[2];
			if(t3 >0 && t3 < 1)
				return true;
		}

		if(t2 > 0.00001 && t2 < 1){
			//System.out.println("Positive Hit");
			if(b[2] + t2*a[2] > 0 && b[2] + t2*a[2] < 1){
				return true;
			}


		}
		if(t1 > 0.00001 && t1 < 1){
			//System.out.println("Positive Hit");
			if(b[2] + t1*a[2] > 0 && b[2] + t1*a[2] < 1){
				return true;
			}
		}
		delete ray;
	}
	return false;
}
