/*
 * TextureMatrix.h
 *
 *  Created on: 16-jan.-2017
 *      Author: arthu
 */

#ifndef MATRICES_TEXTUREMATRIX_H_
#define MATRICES_TEXTUREMATRIX_H_

class TextureMatrix {
public:
	TextureMatrix(int, int);
	virtual ~TextureMatrix();

	double*** array;

	int width;
	int height;

	void generateMatrix(int, int, int);
	void deleteMatrix(int, int, int);
};

#endif /* MATRICES_TEXTUREMATRIX_H_ */
