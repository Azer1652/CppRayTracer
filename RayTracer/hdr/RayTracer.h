/*
 * RayTracer.h
 *
 *  Created on: 30 Sep 2016
 *      Author: arthu
 */

#ifndef RAYTRACER_H_
#define RAYTRACER_H_

#include <string>
#include <thread>

class Tracer;
class Scene;
class ViewMatrix;

static int WIDTH = 800;
static int HEIGHT = 800;

class RayTracer{
	public:
		RayTracer(std::string);
		RayTracer(std::string, int, int);
		virtual ~RayTracer();

		//Speed of Camera Movement
		static double* speed;
		static Scene* scene;

		static ViewMatrix* matrix;

	private:
		void init(std::string, int, int);
		static void framerateControl(int);
		static void display();
		static void reshape(int, int);
		static void keyboard(unsigned char, int, int);

		//Controller ctrl;
		//MyGLEventListener EL;
		//MyKeyListener KL;
		Tracer* tracer = NULL;

		static const unsigned int targetTime = (1/30.0)*1000;

};

#endif /* RAYTRACER_H_ */
