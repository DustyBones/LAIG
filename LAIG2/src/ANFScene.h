#ifndef ANFSCENE_H_
#define ANFSCENE_H_

#include <iostream>
#include <map>
#include <unistd.h>
#include "CGFscene.h"
#include "CGFappearance.h"
#include "CGFapplication.h"
#include "tinyxml.h"
#include "tinystr.h"
#include "Structures.h"

class ANFScene: public CGFscene {
public:
	ANFScene(char *filename);
	~ANFScene();
	void init();
	void update(unsigned long t);
	void display();
	void drawNode(Node* n);
//protected:

	TiXmlDocument* doc;

	TiXmlElement* globalsElement;
	TiXmlElement* camerasElement;
	TiXmlElement* lightsElement;
	TiXmlElement* texturesElement;
	TiXmlElement* appearancesElement;
	TiXmlElement* animationsElement;
	TiXmlElement* graphElement;

	Globals globals;
	map<string, Camera*> cameras;
	map<string, Light> lights;
	map<string, Texture> textures;
	map<string, Appearance> appearances;
	map<string, Animation*> animations;
	Graph graph;

	string initialCam;

};

#endif
