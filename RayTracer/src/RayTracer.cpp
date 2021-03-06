/*
 * GL01Hello.cpp: Test OpenGL C/C++ Setup
 */
#include "../hdr/RayTracer.h"

#include <iostream>

#include <windows.h>  // For MS Windows
#include <GL/Gl.h>
#include <GL/Glu.h>
#include <GL/glut.h>  // GLUT, includes glu.h and gl.h

#include "../hdr/Tracer.h"
#include "../hdr/scene/Scene.h"
#include "../hdr/matrices/TransformMatrix.h"
#include "../hdr/matrices/ViewMatrix.h"

Scene* RayTracer::scene;
ViewMatrix* RayTracer::matrix = new ViewMatrix();
bool RayTracer::drawn = false;
bool RayTracer::reallyDrawn = false;
bool RayTracer::debug = false;

RayTracer::RayTracer(string fileName){
	init(fileName, 4, 1, true);
}

RayTracer::RayTracer(string fileName, int recursion, int blockSize, bool shadows){
	init(fileName, recursion, blockSize, shadows);
}

RayTracer::~RayTracer(){
	delete matrix;
	delete scene;
}

void RayTracer::init(string fileName, int recursion, int blockSize, bool shadows){
	//generate scene
	scene = new Scene(fileName, recursion, blockSize, shadows);

	//Hand scene to tracer
	tracer = RayTracer::scene->getTracer();

	thread myThread = thread([scene]{scene->startTrace(matrix);});

	glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
	glutInitWindowSize(WIDTH, HEIGHT);   // Set the window's initial width & height
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

	glutCreateWindow("C++ Ray Tracer By Janssens Arthur"); // Create a window with the given title

	// openGL init
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); //set to identity
	glMatrixMode(GL_PROJECTION);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // set background (clear) color
	glColor3f(0.0f, 0.0f, 0.0f);
	glPointSize(1.5f);
	gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glutDisplayFunc(display); // Register display callback handler for window re-paint
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutTimerFunc(targetTime, framerateControl, 0);
	glutMainLoop();           // Enter the infinitely event-processing loop


	//Enable window and start tracing
	/*
	EL = new MyGLEventListener(scene);
	//enable key movement (if enabled)
	KL = new MyKeyListener();
	//Add controller
	ctrl = new Controller(tracer ,EL, KL);
	*/

	//init OpenGL


}

void RayTracer::framerateControl(int t){
	if(!reallyDrawn){
		if(drawn){
			reallyDrawn = true;
		}
		glutPostRedisplay();
		glutTimerFunc(targetTime, framerateControl, 0);
	}
}

/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void RayTracer::display() {
	// drawing code
	glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer
	glBegin(GL_POINTS);
	for(int i = 0; i < WIDTH; i++){
		for(int j = 0; j < HEIGHT; j++){
			glColor3d(matrix->p2DArray[i][j][0], matrix->p2DArray[i][j][1], matrix->p2DArray[i][j][2]);
			glVertex2f(i, j);
		}
	}
	glEnd();
	glFlush();
}

void RayTracer::reshape(int width, int height){
	// resized window code

	glutReshapeWindow(WIDTH, HEIGHT);

	/*
	// Set the view port (display area) to cover the entire window
	glViewport(0, 0, WIDTH, HEIGHT);

	// Setup perspective projection, with aspect ratio matches viewport
	glMatrixMode(GL_PROJECTION);  // choose projection matrix
	glLoadIdentity();             // reset projection matrix
	//TODO change width and height to non static
	gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT);
	*/

}

void RayTracer::keyboard(unsigned char key, int x, int y){
	if (key == 27)
		exit(0);
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
	glutInit(&argc, argv);	// Initialize GLUT
	RayTracer* tracer;

	string fileName = "./files/SphereScene.sdl";
	int depth = 4;
	int blockSize = 1;
	bool shadows = true;
	int state = DEBUG;
	for(int i = 1; i < argc; i++){
		switch(state){
		case DEBUG:{
			std::string str(argv[i]);
			if(str.find("-d") != string::npos){
				RayTracer::debug = true;
				state=FILENAME;
			}else{
				fileName=argv[i];
				state=DEPTH;
			}
			break;
		}
		case FILENAME:{
			fileName=argv[i];
			state=DEPTH;
			break;
		}
		case DEPTH:{
			depth = atof(argv[i]);
			state = BLOCKSIZE;
			break;
		}
		case BLOCKSIZE:{
			blockSize = atof(argv[i]);
			state=SHADOWS;
			break;
		}
		case SHADOWS:{
			std::string str(argv[i]);
			if(str.find("false") != string::npos){
				shadows = false;
			}
			state++;
			break;
		}
		default:
			break;
		}
	}

	tracer = new RayTracer(fileName, depth, blockSize, shadows);

	//tracer = new RayTracer("./files/MirrorBoxScene.sdl", 2, 1, true);
	//tracer = new RayTracer("./files/MirrorBoxScene.sdl", 4, 1, true);
	//tracer = new RayTracer("./files/MirrorBoxScene.sdl", 10, 1, true);
	//tracer = new RayTracer("./files/MirrorBoxScene.sdl", 8, 1, true);
	//tracer = new RayTracer("./files/GlassSphereScene.sdl", 4, 1, true);
	//tracer = new RayTracer("./files/RefractionScene.sdl", 4, 1, true);
	//tracer = new RayTracer("./files/RefractionTestScene.sdl", 4, 1, true);
	//tracer = new RayTracer("./files/ToothbrushScene.sdl", 4, 1, true);
	//tracer = new RayTracer("./files/SphereScene.sdl", 4, 1, true);
	//tracer = new RayTracer("./files/ConeScene.sdl", 4, 1, true);
	//tracer = new RayTracer("./files/CylinderScene.sdl", 4, 1, true);
	//tracer = new RayTracer("./files/BoxScene.sdl");
	//tracer = new RayTracer("./files/ConeScene.sdl");
	//tracer = new RayTracer("./files/JanSphereScene.sdl", 4, 1, true);
	//tracer = new RayTracer("./files/BoolObjTest.sdl", 4, 1, true);
	//tracer = new RayTracer("./files/BoolObjCubeTest.sdl", 4, 1, true);
	//tracer = new RayTracer("./files/ComplexBoolObjTest.sdl", 4, 1, true);
	//tracer = new RayTracer("./files/BallInSubstractedCube.sdl", 4, 1, true);
	//tracer = new RayTracer("./files/InsideBallTest.sdl", 4, 1, true);
	//tracer = new RayTracer("./files/MultBoolTest.sdl", 4, 1, true);
	//tracer = new RayTracer("./files/Dice.sdl", 4, 1, true);
	//tracer = new RayTracer("./files/Desk.sdl", 4, 1, true);
	//tracer = new RayTracer("./files/Cube.sdl", 4, 1, true);
	//tracer = new RayTracer("./files/BoolObjInMirrorBox.sdl", 100, 1, true);

	delete tracer;
	return 0;
}
