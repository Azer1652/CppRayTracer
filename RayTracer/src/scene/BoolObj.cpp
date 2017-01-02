/*
 * BoolObj.cpp
 *
 *  Created on: 25 Nov 2016
 *      Author: arthu
 */

#include "../../hdr/scene/BoolObj.h"
#include "../../hdr/Hit.h"
#include <algorithm>

using namespace std;

BoolObj::BoolObj(GeoObj* obj1, GeoObj* obj2, int operation) {
	this->type = BOOLOBJ;
	this->operation = operation;
	objects.push_back(obj1);
	objects.push_back(obj2);

	problemType = NO_OP;
}

BoolObj::~BoolObj() {
	for(GeoObj* obj : objects){
		delete obj;
	}
}

void BoolObj::hit(Ray* r, vector<Hit*>* hitData) {
	vector<Hit*> hits;
	objects[0]->hit(r, &hits);
	objects[1]->hit(r, &hits);
	if(hits.empty()){
		return;
	}
	analyseProblem(&hits);
	selectHits(&hits, hitData);

	//this->setMat(hits.front()->getObj()->getMaterial());

	for (Hit* hit : hits) {
		delete hit;
	}
}

bool BoolObj::hitShadow(Ray* r) {

	vector<Hit*> hits;
	vector<Hit*> hitData;
	objects[0]->hit(r, &hits);
	objects[1]->hit(r, &hits);
	if(hits.empty()){
		return false;
	}
	analyseProblem(&hits);
	selectHits(&hits, &hitData);

	bool shadow = false;

	for(Hit* hit : hitData){
		if(hit->getTime() > 1 || hit->getTime() < 0){
			//do nothing, hit out of reach
		}else{
			shadow = true;
		}
	}
	return shadow;

}

void BoolObj::analyseProblem(vector<Hit*>* hits) {
	//SORT VECTOR
	std::sort(hits->begin(), hits->end(),
			[](const Hit* lhs, const Hit* rhs) {return lhs->time < rhs->time;});
}

void BoolObj::selectHits(vector<Hit*>* hits, std::vector<Hit*>* hitData) {
	switch(operation){
	case ADD:
		switch (this->problemType) {
		case OUT_FIRST:
		case OUT_SECOND:
			hitData->push_back(new Hit(hits->front()));
			hitData->push_back(new Hit(hits->back()));
			break;
		case FIRST_IN:
		case SECOND_IN:
		case BOTH_FIRST:
		case BOTH_SECOND:
		case FIRST_OUT:
		case SECOND_OUT:
			hitData->push_back(new Hit(hits->back()));
			break;
		case NO_OP:
			//ADD ALL
			for (Hit* hit : *hits) {
				hitData->push_back(new Hit(hit));
			}
			break;
		}
		break;
	case SUBSTRACT:
		substractCalc(hits, hitData);
		break;
	case MULTIPLY:
		switch (this->problemType) {
		case OUT_FIRST:
		case OUT_SECOND:
			hitData->push_back(new Hit(hits->at(1)));
			hitData->push_back(new Hit(hits->at(2)));
			break;
		case FIRST_OUT:
			//none
			break;
		case FIRST_IN:
		case SECOND_IN:
			hitData->push_back(new Hit(hits->front()));
			hitData->push_back(new Hit(hits->at(1)));
			break;
		case BOTH_FIRST:
		case BOTH_SECOND:
			hitData->push_back(new Hit(hits->front()));
			break;
		case SECOND_OUT:
			//none
			break;
		case NO_OP:
			//not existant
			break;
		}
		break;
	}
}

void BoolObj::substractCalc(std::vector<Hit*>* hits, std::vector<Hit*>* hitData){
	if(hits->size() == 1){
		if(hits->front()->obj == objects[0] && !hits->front()->getEntering())
			hitData->push_back(new Hit(hits->front()));
		if(hits->front()->obj == objects[1] && hits->front()->getEntering())
			hitData->push_back(new Hit(hits->front()));
		return;
	}else{
		Hit* prev = NULL;
		for(Hit* curr : *hits){
			if(prev == NULL){
				prev = curr;
				continue;
			}
			//first obj enter followed by first object exit == logical
			if(prev->getObj() == objects[0] && prev->getEntering() && curr->getObj() == objects[0] && !curr->getEntering()){
				hitData->push_back(new Hit(prev));
				hitData->push_back(new Hit(curr));
			}

			//first obj In followed by Second obj In
			if(prev->getObj() == objects[0] && prev->getEntering() && curr->getObj() == objects[1] && curr->getEntering()){
				hitData->push_back(new Hit(prev));
				hitData->push_back(new Hit(curr));
			}

			//second obj out followed by first obj out
			if(prev->getObj() == objects[1] && !prev->getEntering() && curr->getObj() == objects[0] && !curr->getEntering()){
				hitData->push_back(new Hit(prev));
				double* normal = hitData->back()->getNormal();
				normal[0] = -normal[0];
				normal[1] = -normal[1];
				normal[2] = -normal[2];
				hitData->push_back(new Hit(curr));
			}
			prev = curr;
		}
	}
}
