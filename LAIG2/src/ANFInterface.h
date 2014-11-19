#ifndef ANFINTERFACE_H
#define ANFINTERFACE_H

#include "ANFScene.h"
#include "CGFinterface.h"
#include "Structures.h"

class ANFInterface: public CGFinterface {
public:
		virtual void initGUI();
		virtual void processGUI(GLUI_Control *ctrl);
		virtual void processKeyboard(unsigned char key, int x, int y);
};

#endif
