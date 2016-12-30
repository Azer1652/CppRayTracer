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

	for (Hit* hit : hits) {
		delete hit;
	}
}

bool BoolObj::hitShadow(Ray* r) {
	/*if(this->operation == ADD){
		if(objects[0]->hitShadow(r) ||	objects[1]->hitShadow(r)){
			return true;
		}
	}else{
		vector<Hit*> hits;
		objects[0]->hit(r, &hits);
		objects[1]->hit(r, &hits);
		if(hits.empty()){
			return false;
		}
		analyseProblem(&hits);
		vector<Hit*> hitData;
		selectHits(&hits, &hitData);
		for(Hit* hit : hitData){
			if(hit->getObj() == objects.at(0)){
				for (Hit* hit : hits) {
					delete hit;
				}
				for (Hit* hit : hitData) {
					delete hit;
				}
				return true;
			}
		}
	}*/
	return false;
}

void BoolObj::analyseProblem(vector<Hit*>* hits) {
	//SORT VECTOR
	std::sort(hits->begin(), hits->end(),
			[](const Hit* lhs, const Hit* rhs) {return lhs->time < rhs->time;});

	bool logical = true;
	int i = 0;
	for (Hit* hit : *hits) {
		switch (i % 2) {
		case 0:
			if (!hit->getEntering()) {
				logical = false;
			}
			i++;
			break;
		case 1:
			if (hit->getEntering()) {
				logical = false;
			}
			i++;
			break;
		}
	}

	if (!logical) {
		if(hits->size() == 1){
			if(hits->at(0)->getObj() == objects.at(0))
				problemType = FIRST_OUT;
			else
				problemType = SECOND_OUT;
		}else if(hits->size() == 2){
			if(hits->at(0)->getObj() == objects.at(0))
				problemType = BOTH_FIRST;
			else
				problemType = BOTH_SECOND;
		}if(hits->size() == 3){
			if(hits->at(0)->getObj() == objects.at(0))
				problemType = SECOND_IN;
			else
				problemType = FIRST_IN;
		}if(hits->size() == 4){
			if(hits->at(0)->getObj() == objects.at(0))
				problemType = OUT_FIRST;
			else
				problemType = OUT_SECOND;
		}
	} else {
		problemType = NO_OP;
	}
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
		switch (this->problemType) {
		case OUT_FIRST:
			hitData->push_back(new Hit(hits->front()));
			hitData->push_back(new Hit(hits->at(1)));
			break;
		case FIRST_OUT:
		case FIRST_IN:
			hitData->push_back(new Hit(hits->front()));
			break;
		case BOTH_FIRST:
			hitData->push_back(new Hit(hits->front()));
			hitData->push_back(new Hit(hits->at(1)));
			break;
		case BOTH_SECOND:
		case SECOND_OUT:
			//none
			break;
		case SECOND_IN:
			hitData->push_back(new Hit(hits->at(1)));
			hitData->push_back(new Hit(hits->back()));
			break;
		case OUT_SECOND:
			hitData->push_back(new Hit(hits->at(2)));
			hitData->push_back(new Hit(hits->back()));
			break;
		case NO_OP:
			//ADD ALL IF OBJ 1 HIT
			for (Hit* hit : *hits) {
				if(hit->obj == objects.at(0) && hit->getEntering() == true)
					hitData->push_back(new Hit(hit));
			}
			break;
		}
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
