/*
 * Obj.h
 *
 *  Created on: 14 Oct 2016
 *      Author: arthu
 */

#ifndef OBJ_H_
#define OBJ_H_

class Obj {
public:
	Obj();
	virtual ~Obj();
	double* pos;
	double* color;

	void setPosition(double*);
	double* getPosition(){return pos;};
	void setColor(double*);
	double* getColor(){return color;};
};

#endif /* OBJ_H_ */
