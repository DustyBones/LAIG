#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdlib.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <string>
#include <vector>
#include <map>
#include <math.h>
#include <CGFapplication.h>
#include <CGFshader.h>
#include <CGFtexture.h>

class Drawing {
public:
	string mode, shading;
	float background[4];
};

class Culling {
public:
	string face, order;
};

class Lighting {
public:
	bool dsided, local, enabled;
	float ambient[4];
};

class Globals {
public:
	Drawing drawing;
	Culling culling;
	Lighting lighting;
};

class Camera {
public:
	string id;
	virtual void apply()=0;
};

class CamPerspective: public Camera {
public:
	float near, far, angle, pos[3], target[3];
	void apply(){
		float ratio = ((float) CGFapplication::width) /
			((float) CGFapplication::height);
		glPushMatrix();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(angle, ratio, near, far);
		glPopMatrix();
		gluLookAt(pos[0], pos[1], pos[2], target[0], target[1], target[2], 0, 1, 0);
	}
};

class CamOrtho: public Camera {
public:
	string direction;
	float near, far, left, right, top, bottom;
	void apply() {
		float ratio = ((float) CGFapplication::width) /
		((float) CGFapplication::height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(left * ratio, right * ratio, bottom, top, near, far);
		if (direction == "x"){
			glRotatef(-90, 0, 1, 0);
		}
		if (direction == "y"){
			glRotatef(90, 1, 0, 0);
		}
	}
};

class Light {
public:
	string id, type;
	float pos[3], target[3], angle, exponent, component[3][4];
	bool enabled, marker;
	CGFlight* light;
};

class Texture {
public:
	string id, file;
	float length_s, length_t;
};

class Appearance {
public:
	string id, textureref;
	CGFappearance* ap;
};

class Animation {
public:
	string id;
	float start_time, current_time, span;
	virtual void init(unsigned int t)=0;
	virtual void update(unsigned int t)=0;
	virtual void apply()=0;
};

class LinearAnimation: public Animation{
public:
	vector< vector<float> > controlPoint;
	float total_dist, current_dist, step_dist, v[3], ang,a;
	void init(unsigned int t){
		start_time=t;
		total_dist=0;
		for(size_t i=1; i<controlPoint.size(); ++i){
			total_dist+=sqrt((controlPoint[i][0]-controlPoint[i-1][0])
				*(controlPoint[i][0]-controlPoint[i-1][0])
				+(controlPoint[i][1]-controlPoint[i-1][1])
				*(controlPoint[i][1]-controlPoint[i-1][1])
				+(controlPoint[i][2]-controlPoint[i-1][2])
				*(controlPoint[i][2]-controlPoint[i-1][2]));
		}
		current_dist=0;
		step_dist= total_dist/(span*1000); //speed
	}
	void update(unsigned int t){
		float d=0;
		current_time=t-start_time;
		current_dist=t*step_dist;
	}
	void apply(){
		glTranslatef(v[0], v[1], v[2]);
		glRotatef(a, 0,1,0);
	}
};

class CircularAnimation: public Animation {
public:
	float center[3], radius, startang, rotang, step_angle, current_rotation_angle;
	void init(unsigned int t){
		start_time=t;
		step_angle=rotang/(span*1000);
	}
	void update(unsigned int t){
		current_time=t-start_time;
		current_rotation_angle=startang + current_time*step_angle;
	}
	void apply(){
		glTranslatef(center[0],center[1],center[2]);
		glRotatef(current_rotation_angle, 0,1,0);
		glTranslatef(-center[0],-center[1],-center[2]);
	}
};

class Primitive {
public:
	virtual void draw() = 0;
};

class Triangle: public Primitive {
public:
	float xyz1[3], xyz2[3], xyz3[3], normal[3];
	void calcNormal(){
		float u[3], v[3];
		u[0]=xyz2[0]-xyz1[0]; u[1]=xyz2[1]-xyz1[1]; u[2]=xyz2[2]-xyz1[2];
		v[0]=xyz3[0]-xyz1[0]; v[1]=xyz3[1]-xyz1[1]; v[2]=xyz3[2]-xyz1[2];
		normal[0]=u[1]*v[2]-u[2]*v[1];
		normal[1]=u[2]*v[0]-u[0]*v[2];
		normal[2]=u[0]*v[1]-u[0]*v[1];
	}
	void draw() {
		glBegin (GL_TRIANGLES);
		glNormal3f(normal[0],normal[1], normal[2]);
		glTexCoord2f(0,0);
		glVertex3f(xyz1[0], xyz1[1], xyz1[2]);
		glTexCoord2f(1,0);
		glVertex3f(xyz2[0], xyz2[1], xyz2[2]);
		glTexCoord2f(0.5,0.5);
		glVertex3f(xyz3[0], xyz3[1], xyz3[2]);
		glEnd();
	}
};

class Rectangle: public Primitive {
public:
	float xy1[2], xy2[2];
	void draw() {
		glBegin (GL_POLYGON);
		glNormal3f(0,0,1);
		glTexCoord2f(0,0);
		glVertex2f(xy1[0], xy1[1]);
		glTexCoord2f(1,0);
		glVertex2f(xy2[0], xy1[1]);
		glTexCoord2f(1,1);
		glVertex2f(xy2[0], xy2[1]);
		glTexCoord2f(0,1);
		glVertex2f(xy1[0], xy2[1]);
		glEnd();
	}
};

class Cylinder: public Primitive {
public:
	float base, top, height;
	int slices, stacks;
	void draw() {
		gluCylinder(gluNewQuadric(), base, top, height, slices, stacks);
	}
};

class Sphere: public Primitive {
public:
	float radius;
	int slices, stacks;
	void draw() {
		gluSphere(gluNewQuadric(), radius, slices, stacks);
	}
};

class Torus: public Primitive {
public:
	float inner, outer;
	int slices, loops;
	void draw() {
		glutSolidTorus(inner, outer, slices, loops);
	}
};

class Plane: public Primitive {
public:
	int parts;
	void draw(){
		//init
		GLfloat controlPoint[4][3]={{0.5,0,0.5}, {-0.5,0,0.5}, {0.5,0,-0.5}, {-0.5,0,-0.5}};
		GLfloat normalPoint[4][3]={{0.0,1.0,0.0}, {0.0,1.0,0.0}, {0.0,1.0,0.0}, {0.0,1.0,0.0}};
		GLfloat texturePoint[4][2]={{1.0,0.0}, {0.0,0.0}, {1.0,1.0}, {0.0,1.0}};

		glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 2, 0.0, 1.0, 6, 2, &controlPoint[0][0]);
		glMap2f(GL_MAP2_NORMAL, 0.0, 1.0, 3, 2, 0.0, 1.0, 6, 2, &normalPoint[0][0]);
		glMap2f(GL_MAP2_TEXTURE_COORD_2, 0.0, 1.0, 2, 2, 0.0, 1.0, 4, 2, &texturePoint[0][0]);

		glEnable(GL_MAP2_VERTEX_3);
		glEnable(GL_MAP2_NORMAL);
		glEnable(GL_MAP2_TEXTURE_COORD_2);

		glMapGrid2f(parts, 0.0, 1.0, parts, 0.0, 1.0);
		//display
		glEvalMesh2(GL_FILL, 0, parts, 0, parts);
	}
};

class Patch: public Primitive {
public:
	int order, partsU, partsV;
	string compute;
	GLfloat controlPoint[16][3], texturePoint[16][2];
	void draw(){
		//init
		int interpol = order+1;
		glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3*interpol, interpol, 0.0, 1.0, 3, interpol, &controlPoint[0][0]);
		glMap2f(GL_MAP2_TEXTURE_COORD_2, 0.0, 1.0, 2*interpol, interpol, 0.0, 1.0, 2, interpol, &texturePoint[0][0]);

		glEnable(GL_MAP2_VERTEX_3);
		glEnable(GL_AUTO_NORMAL);
		glEnable(GL_MAP2_TEXTURE_COORD_2);

		glMapGrid2f(partsU, 0.0, 1.0, partsV, 0.0, 1.0);

		//display
		glFrontFace(GL_CW);
		if(compute=="point"){
			glEvalMesh2(GL_POINT, 0, partsU, 0, partsV);
		}else if(compute == "line"){
			glEvalMesh2(GL_LINE, 0, partsU, 0, partsV);
		}else{
			glEvalMesh2(GL_FILL, 0, partsU, 0, partsV);
		}
		glFrontFace(GL_CCW);
	}
};

class Vehicle: public Primitive{
public:
	vector<Plane*> planes;
	vector<Patch*> patches;
	Vehicle(){
		;
	}
	void draw(){
		for(size_t i=0; i<planes.size(); ++i){
			planes[i]->draw();
		}
		for(size_t i=0; i<patches.size(); ++i){
			planes[i]->draw();
		}
	}
};

class Flag: public Primitive, CGFshader{
public:
	Plane* p;
	CGFtexture* t;
	GLint timeLoc, windLoc, textureLoc;
	unsigned int time;
	int wind;
	Flag(string text){
		p=new Plane();
		p->parts=100;
		init("../data/flag.vert", "../data/flag.frag");
		CGFshader::bind();
		time=0;
		timeLoc = glGetUniformLocation(id(), "time");
		wind=1;
		windLoc = glGetUniformLocation(id(), "wind");
		t = new CGFtexture("../data/"+text);
		textureLoc = glGetUniformLocation(id(), "textureImage");
		glUniform1i(textureLoc, 0);
	}
	void update(unsigned int t, int w){
		time=t;
		wind=w;
	}

	void bind(){
		CGFshader::bind();
		// update uniforms
		glUniform1i(timeLoc, time);

		// make sure the correct texture unit is active
		glActiveTexture(GL_TEXTURE0);

		// apply/activate the texture you want, so that it is bound to GL_TEXTURE0
		t->apply();
		p->draw();
		CGFshader::unbind();
	}

	void draw(){
		bind();
	}
};

class Node {
public:
	string id, appearanceref;
	vector<string> animationref;
	bool displaylist;
	float matrix[16];
	vector<Primitive*> primitives;
	vector<string> descendants;
};

class Graph {
public:
	string rootid, initialCam;
	map<string, Node*> nodes;
};

#endif
