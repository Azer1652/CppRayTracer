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

vector<GeoObj*>* BoolObj::getObjects(){
	return &objects;
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
		addCalc(hits, hitData);
		break;
	case SUBSTRACT:
		substractCalc(hits, hitData);
		break;
	case MULTIPLY:
		multCalc(hits, hitData);
		break;
	}
}

void BoolObj::addCalc(std::vector<Hit*>* hits, std::vector<Hit*>* hitData){
	if(hits->size() == 1){
		hitData->push_back(new Hit(hits->front()));
	}else{
		hitData->push_back(new Hit(hits->front()));
		hitData->push_back(new Hit(hits->back()));
	}
}

void BoolObj::substractCalc(std::vector<Hit*>* hits, std::vector<Hit*>* hitData){
	if(hits->size() == 1){
		if(compareObjectsComplex(hits->front()->obj, objects[0]) && !hits->front()->getEntering())
			hitData->push_back(new Hit(hits->front()));
		if(compareObjectsComplex(hits->front()->obj, objects[1]) && hits->front()->getEntering())
			hitData->push_back(new Hit(hits->front()));
		return;
	}else{
		bool encapsulated = false;
		Hit* prev = NULL;
		//Analyze all hits
		//Delete duplicates
		Hit* curr = NULL;
		for(vector<Hit*>::iterator it = hits->begin(); it!=hits->end();){
			curr = (*it);
			if(prev == NULL){
				prev = curr;
				it++;
				continue;
			}
			if(abs(prev->getTime()-curr->getTime())<0.00000001){
				if(compareObjectsComplex(curr->getObj(), this)){
					it = hits->erase(it-1);
					it++;
				}else{
					it = hits->erase(it);
				}

			}else{
				prev = curr;
				it++;
			}
		}

		//Look for patterns
		vector<Hit*> hitBuffer;
		prev = NULL;
		for(Hit* curr : *hits){
			if(prev == NULL){
				prev = curr;
				continue;
			}

			if(compareObjectsComplex(prev->getObj(), objects[1]) && prev->getEntering()){
				encapsulated = true;
			}

			if(compareObjectsComplex(prev->getObj(), objects[1]) && !prev->getEntering()){
				encapsulated = false;
			}

			if(!encapsulated){
				//first obj enter followed by first object exit == logical
				if(compareObjectsComplex(prev->getObj(), objects[0]) && prev->getEntering() && compareObjectsComplex(curr->getObj(), objects[0]) && !curr->getEntering()){
					hitBuffer.push_back(new Hit(prev));
					hitBuffer.push_back(new Hit(curr));
				}

				//first obj In followed by Second obj In
				if(compareObjectsComplex(prev->getObj(), objects[0]) && prev->getEntering() && compareObjectsComplex(curr->getObj(), objects[1]) && curr->getEntering()){
					hitBuffer.push_back(new Hit(prev));
					Hit* newHit = new Hit(curr);
					newHit->setEntering(false);
					//newHit->flipNormal();
					hitBuffer.push_back(newHit);
				}

				//second obj out followed by first obj out
				if(compareObjectsComplex(prev->getObj(), objects[1]) && !prev->getEntering() && compareObjectsComplex(curr->getObj(), objects[0]) && !curr->getEntering()){
					Hit* prevHit = new Hit(prev);
					prevHit->setEntering(true);
					//prevHit->flipNormal();
					hitData->push_back(prevHit);
					hitData->push_back(new Hit(curr));
				}

				//if second object found exiting without enter, all previous hits are invalidated
				if(compareObjectsComplex(curr->getObj(), objects[1]) && !curr->getEntering()){
					for(Hit* hit : hitBuffer)
						delete hit;
					hitBuffer.clear();
				}

				//previous ones were really not encapsulated by second object -> add them
				if(compareObjectsComplex(curr->getObj(), objects[1]) && curr->getEntering()){
					for(Hit* hit : hitBuffer)
						hitData->push_back(hit);
					hitBuffer.clear();
				}
			}


			prev = curr;
		}
		if(!hitBuffer.empty()){
			for(Hit* hit : hitBuffer)
				hitData->push_back(hit);
			hitBuffer.clear();
		}
	}
}

void BoolObj::multCalc(std::vector<Hit*>* hits, std::vector<Hit*>* hitData){
	if(hits->size() == 1){
		//none
	}else if(hits->size() == 2){
		if(compareObjectsComplex(hits->front()->obj, objects[0]) && !hits->front()->getEntering())
			hitData->push_back(new Hit(hits->front()));
		if(compareObjectsComplex(hits->front()->obj, objects[1]) && !hits->front()->getEntering())
			hitData->push_back(new Hit(hits->front()));
		return;
	}else{
		bool encapsulated = false;
		Hit* prev = NULL;
		//Analyze all hits
		//Delete duplicates
		Hit* curr = NULL;
		for(vector<Hit*>::iterator it = hits->begin(); it!=hits->end();){
			curr = (*it);
			if(prev == NULL){
				prev = curr;
				it++;
				continue;
			}
			if(abs(prev->getTime()-curr->getTime())<0.00000001){
				if(compareObjectsComplex(curr->getObj(), this)){
					it = hits->erase(it-1);
					it++;
				}else{
					it = hits->erase(it);
				}

			}else{
				prev = curr;
				it++;
			}
		}

		//Look for patterns
		vector<Hit*> hitBuffer;
		prev = NULL;
		for(Hit* curr : *hits){
			if(prev == NULL){
				prev = curr;
				continue;
			}

			if((compareObjectsComplex(prev->getObj(), objects[1]) && prev->getEntering()) ||
				(compareObjectsComplex(prev->getObj(), objects[0]) && prev->getEntering())){
				encapsulated = true;
			}

			if((compareObjectsComplex(prev->getObj(), objects[1]) && !prev->getEntering()) ||
				(compareObjectsComplex(prev->getObj(), objects[0]) && !prev->getEntering())){
				encapsulated = false;
			}

			if(!encapsulated){
				//first obj In followed by Second obj out
				if(compareObjectsComplex(prev->getObj(), objects[0]) && prev->getEntering() && compareObjectsComplex(curr->getObj(), objects[1]) && !curr->getEntering()){
					hitBuffer.push_back(new Hit(prev));
					Hit* newHit = new Hit(curr);
					newHit->setEntering(false);
					//newHit->flipNormal();
					hitBuffer.push_back(newHit);
				}

				//second obj in followed by first obj out
				if(compareObjectsComplex(prev->getObj(), objects[1]) && prev->getEntering() && compareObjectsComplex(curr->getObj(), objects[0]) && !curr->getEntering()){
					Hit* prevHit = new Hit(prev);
					prevHit->setEntering(true);
					//prevHit->flipNormal();
					hitData->push_back(prevHit);
					hitData->push_back(new Hit(curr));
				}

				//if second object found exiting without enter, all previous hits true
				if(compareObjectsComplex(curr->getObj(), objects[1]) && !curr->getEntering()){
					for(Hit* hit : hitBuffer)
						hitData->push_back(hit);
					hitBuffer.clear();

				}

				//previous ones were really not encapsulated by second object -> invalidated
				if(compareObjectsComplex(curr->getObj(), objects[1]) && curr->getEntering()){
					for(Hit* hit : hitBuffer)
						delete hit;
					hitBuffer.clear();
				}
			}else{	//encapsulated
				//just add current Hit
				hitBuffer.push_back(new Hit(curr));
			}


			prev = curr;
		}
		if(!hitBuffer.empty()){
			for(Hit* hit : hitBuffer)
				hitData->push_back(hit);
			hitBuffer.clear();
		}
	}
}

bool BoolObj::compareObjectsComplex(GeoObj* first, GeoObj* second){
	if(first->type == BOOLOBJ){
		if(second->type == BOOLOBJ){
			BoolObj* obj = (BoolObj*)first;
			BoolObj* obj2 = (BoolObj*)second;
			if(compareObjectsComplex(obj->getObjects()->at(0) ,obj2->getObjects()->at(0)) ||
					compareObjectsComplex(obj->getObjects()->at(1), obj2->getObjects()->at(0)) ||
					compareObjectsComplex(obj->getObjects()->at(0), obj2->getObjects()->at(1)) ||
					compareObjectsComplex(obj->getObjects()->at(1), obj2->getObjects()->at(1)))
				return true;
		}else{
			BoolObj* obj = (BoolObj*)first;
			if(compareObjectsComplex(obj->getObjects()->at(0), second) || compareObjectsComplex(obj->getObjects()->at(1), second))
				return true;
		}
	}else{
		if(second->type == BOOLOBJ){
			BoolObj* obj2 = (BoolObj*)second;
			if(compareObjectsComplex(obj2->getObjects()->at(0), first) || compareObjectsComplex(obj2->getObjects()->at(1), first))
				return true;
		}else{
			if(first == second)
				return true;
		}
	}
	return false;
}
