#ifndef ANFINTERFACE_H
#define ANFINTERFACE_H

#include "Structures.h"
#include "ANFScene.h"
#include "CGFinterface.h"

class ANFInterface: public CGFinterface {
private:
	ANFScene* scene;
public:
	ANFInterface(ANFScene* scene){
		this->scene=scene;
	}
	virtual void initGUI();
	virtual void processGUI(GLUI_Control *ctrl);
	virtual void processKeyboard(unsigned char key, int x, int y);
};

#endif
