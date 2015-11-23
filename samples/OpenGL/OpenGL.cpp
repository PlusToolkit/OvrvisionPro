//
//

#include "ovrvision_pro.h"

#include <GL/gl.h> 
#include <GL/glu.h> 

void SWAPBUFFERS();	// platform depend function

#define BLACK_INDEX     0 
#define RED_INDEX       13 
#define GREEN_INDEX     14 
#define BLUE_INDEX      16 


/* OpenGL globals, defines, and prototypes */ 
GLfloat latitude, longitude, latinc, longinc; 
GLdouble radius; 
GLuint texture;

OVR::OvrvisionPro camera;	// OvrvisionPro camera
//VideoCapture camera; // dummy camera

#define GLOBE    1 
#define CYLINDER 2 
#define CONE     3 


void polarView( GLdouble, GLdouble, GLdouble, GLdouble); 
GLvoid createObjects();

/* OpenGL code */
GLvoid initializeGL(GLsizei width, GLsizei height)
{
	GLfloat     maxObjectSize, aspect;
	GLdouble    near_plane, far_plane;

	glClearIndex((GLfloat)BLACK_INDEX);
	glClearDepth(1.0);

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	aspect = (GLfloat)width / height;
	gluPerspective(45.0, aspect, 3.0, 7.0);
	glMatrixMode(GL_MODELVIEW);

	near_plane = 3.0;
	far_plane = 7.0;
	maxObjectSize = 3.0F;
	radius = near_plane + maxObjectSize / 2.0;

	latitude = 0.0F;
	longitude = 0.0F;
	latinc = 6.0F;
	longinc = 2.5F;

	if (camera.Open(0, OVR::Camprop::OV_CAMHD_FULL) == 0)
		puts("Can't open OvrvisionPro");

	createObjects();
}

GLvoid resize(GLsizei width, GLsizei height)
{
	GLfloat aspect;

	glViewport(0, 0, width, height);

	aspect = (GLfloat)width / height;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, aspect, 3.0, 7.0);
	glMatrixMode(GL_MODELVIEW);
}

GLvoid createObjects()
{
	GLUquadricObj *quadObj;

	glNewList(GLOBE, GL_COMPILE);
	quadObj = gluNewQuadric();
	gluQuadricDrawStyle(quadObj, GLU_LINE);
	gluSphere(quadObj, 1.5, 16, 16);
	glEndList();

	glNewList(CONE, GL_COMPILE);
	quadObj = gluNewQuadric();
	gluQuadricDrawStyle(quadObj, GLU_FILL);
	gluQuadricNormals(quadObj, GLU_SMOOTH);
	gluCylinder(quadObj, 0.3, 0.0, 0.6, 15, 10);
	glEndList();

	glNewList(CYLINDER, GL_COMPILE);
	glPushMatrix();
	glRotatef((GLfloat)90.0, (GLfloat)1.0, (GLfloat)0.0, (GLfloat)0.0);
	glTranslatef((GLfloat)0.0, (GLfloat)0.0, (GLfloat)-1.0);
	quadObj = gluNewQuadric();
	gluQuadricDrawStyle(quadObj, GLU_FILL);
	gluQuadricNormals(quadObj, GLU_SMOOTH);
	gluCylinder(quadObj, 0.3, 0.3, 0.6, 12, 2);
	glPopMatrix();
	glEndList();
}


void polarView(GLdouble radius, GLdouble twist, GLdouble latitude,
	GLdouble longitude)
{
	glTranslated(0.0, 0.0, -radius);
	glRotated(-twist, 0.0, 0.0, 1.0);
	glRotated(-latitude, 1.0, 0.0, 0.0);
	glRotated(longitude, 0.0, 0.0, 1.0);

}

GLvoid drawScene(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	latitude += latinc;
	longitude += longinc;

	polarView(radius, 0, latitude, longitude);

	glIndexi(RED_INDEX);
	glCallList(CONE);

	glIndexi(BLUE_INDEX);
	glCallList(GLOBE);

	glIndexi(GREEN_INDEX);
	glPushMatrix();
	glTranslatef(0.8F, -0.65F, 0.0F);
	glRotatef(30.0F, 1.0F, 0.5F, 1.0F);
	glCallList(CYLINDER);
	glPopMatrix();

	glPopMatrix();

	SWAPBUFFERS();
}