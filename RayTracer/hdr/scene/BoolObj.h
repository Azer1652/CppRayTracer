/*
 * BoolObj.h
 *
 *  Created on: 25 Nov 2016
 *      Author: arthu
 */

#ifndef BOOLOBJ_H_
#define BOOLOBJ_H_

#include "GeoObj.h"
#include <vector>

enum{
	ADD,
	SUBSTRACT,
	MULTIPLY
};

enum{
	OUT_FIRST,
	FIRST_OUT,
	FIRST_IN,
	BOTH_FIRST,
	BOTH_SECOND,
	SECOND_OUT,
	SECOND_IN,
	OUT_SECOND,
	NO_OP
};

class BoolObj : public GeoObj{
public:
	BoolObj(GeoObj*, GeoObj*, int);
	virtual ~BoolObj();

	void hit(Ray*, std::vector<Hit*>*);
	bool hitShadow(Ray*);

	int problemType;

	bool compare(const Hit&, const Hit&);

	std::vector<GeoObj*>* getObjects();
private:
	std::vector<GeoObj*> objects;
	int operation;

	void analyseProblem(std::vector<Hit*>*);
	void selectHits(std::vector<Hit*>*, std::vector<Hit*>*);

	void addCalc(std::vector<Hit*>*, std::vector<Hit*>*);
	void substractCalc(std::vector<Hit*>*, std::vector<Hit*>*);
	void multCalc(std::vector<Hit*>*, std::vector<Hit*>*);

	bool compareObjectsComplex(GeoObj*, GeoObj*);
};

#endif /* BOOLOBJ_H_ */
