/*
 * Bar.cpp
 *
 *  Created on: 5 Oct 2016
 *      Author: arthu
 */

#include "../../hdr/scene/Bar.h"
#include "../../hdr/Lib.h"
#include "../../hdr/Ray.h"
#include "../../hdr/Hit.h"

#include <cmath>

using namespace std;

Bar::Bar() {
	this->type=BAR;
}

Bar::~Bar() {
	// TODO Auto-generated destructor stub
}

void Bar::hit(Ray* r, vector<Hit*>* hitData){
	//returns closest hitpoint for given ray
	Ray* ray = new Ray();
	Lib::xfrmRay(ray, this->getInverseTransform(), r);
	double tHit = -1, numer = 0, denom = 0;
	double tIn=-100000.0, tOut = 100000.0;
	int inSurf = 0, outSurf = 0;
	for(int i=0; i<6; i++){
		switch(i){
			case 0: {
				numer=1.0-ray->getLocation()[1];
					denom= ray->getDirection()[1];
					break;
			}
			case 1:{
				numer=1.0+ray->getLocation()[1];
					denom=-ray->getDirection()[1];
					break;
			}
			case 2: {
					numer=1.0-ray->getLocation()[0];
					denom=ray->getDirection()[0];
					break;
			}
			case 3: {
				numer=1.0+ray->getLocation()[0];
					denom=-ray->getDirection()[0];
					break;
			}
			case 4: {
				numer=1.0-ray->getLocation()[2];
					denom=ray->getDirection()[2];
					break;
			}
			case 5:{
				numer=1.0+ray->getLocation()[2];
					denom=-ray->getDirection()[2];
					break;
			}
		}

		if(abs(denom)<0.00001){	//Ray is parallel
			if(numer<0){
				delete ray;
				return; //HitData will be empty
			}//ray inside, no change to tIn, tOut
		}else{//ray is not parallel
			tHit = numer/denom;
			if(denom>0){//exiting
				if(tHit<tOut){//a new earlier exit
					tOut=tHit;
					outSurf=i;
				}
			}else{//denom is negative: entering
				if(tHit>tIn){//a new later entrance
					tIn=tHit;
					inSurf=i;
				}
			}
		}
		if(tIn>=tOut){
			delete ray;
			return;// it's a miss- early out
		}
	}//end of the for loop
	if(tIn > 0.000001){
		//System.out.println("Positive Hit");
		Hit* hit = new Hit();
		hit->setPosition(new double[3]{r->getLocation()[0]+r->getDirection()[0]*tIn,
						r->getLocation()[1]+r->getDirection()[1]*tIn,
						r->getLocation()[2]+r->getDirection()[2]*tIn});
		hit->setTime(tIn);
		hit->setObj(this);
		hit->setEntering(true);
		hit->setNormal(getNormal(inSurf));
		hitData->push_back(hit);
	}
	if(tOut > 0.000001){
		//System.out.println("Positive Hit");
		Hit* hit = new Hit();
		hit->setPosition(new double[3]{r->getLocation()[0]+r->getDirection()[0]*tOut,
						r->getLocation()[1]+r->getDirection()[1]*tOut,
						r->getLocation()[2]+r->getDirection()[2]*tOut});
		hit->setTime(tOut);
		hit->setObj(this);
		hit->setEntering(false);
		hit->setNormal(getNormal(outSurf));
		hitData->push_back(hit);
	}
	delete ray;
	return;
}

bool Bar::hitShadow(Ray* r){
	Ray* ray = new Ray();
	//returns closest hitpoint for given ray
	Lib::xfrmRay(ray, this->getInverseTransform(), r);

	double tHit = -1, numer = 0, denom = 0;
	double tIn=-100000.0, tOut = 100000.0;
	for(int i=0; i<6; i++)
	{
		switch(i)
		{
			case 0: {
				numer=1.0-ray->getLocation()[1];
					denom= ray->getDirection()[1];
					break;
			}
			case 1:{
				numer=1.0+ray->getLocation()[1];
					denom=-ray->getDirection()[1];
					break;
			}
			case 2: {
					numer=1.0-ray->getLocation()[0];
					denom=ray->getDirection()[0];
					break;
			}
			case 3: {
				numer=1.0+ray->getLocation()[0];
					denom=-ray->getDirection()[0];
					break;
			}
			case 4: {
				numer=1.0-ray->getLocation()[2];
					denom=ray->getDirection()[2];
					break;
			}
			case 5:{
				numer=1.0+ray->getLocation()[2];
					denom=-ray->getDirection()[2];
					break;
			}
		}

		if(abs(denom)<0.00001){	//Ray is parallel
			if(numer<0){
				delete ray;
				return false; //HitData will be empty
			}//ray inside, no change to tIn, tOut
		}else//ray is not parallel
		{
			tHit = numer/denom;
			if(denom>0){//exiting
				if(tHit<tOut)//a new earlier exit
				{
					tOut=tHit;
				}
			}
			else{//denom is negative: entering
				if(tHit>tIn)//a new later entrance
				{
					tIn=tHit;
				}
			}
		}
		if (tIn>=tOut){
			delete ray;
			return false;// it's a miss- early out
		}
	}//end of the for loop
	delete ray;
	if(tIn > 0.000001 && tIn < 1){
		//System.out.println("Positive Hit");
		return true;
	}
	if(tOut > 0.000001 && tOut < 1){
		//System.out.println("Positive Hit");
		return true;
	}
	return false;
}

double* Bar::getNormal(int surface){
	switch(surface)
	{
		case 0:{
			return new double[3]{0,1,0};
		}
		case 1:{
			return new double[3]{0,-1,0};
		}
		case 2:{
			return new double[3]{1,0,0};
		}
		case 3:{
			return new double[3]{-1,0,0};
		}
		case 4:{
			return new double[3]{0,0,1};
		}
		case 5:{
			return new double[3]{0,0,-1};
		}
	}
	return NULL;
}
