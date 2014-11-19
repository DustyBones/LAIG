#include "ANFInterface.h"

void ANFInterface::processKeyboard(unsigned char key, int x, int y)
{
	// Uncomment below if you would like to process the default keys (e.g. 's' for snapshot, 'Esc' for exiting, ...)
	CGFinterface::processKeyboard(key, x, y);
}

void ANFInterface::initGUI()
{
	// Check CGFinterface.h and GLUI documentation for the types of controls available
	/*GLUI_Panel *varPanel= addPanel("Cameras", 1);
  int id=100;
  map<string, Camera*>::iterator it_cam;
  for(it_cam=&(((ANFScene*) scene)->cameras.begin());it_cam!=&(((ANFScene*) scene)->cameras.end()); ++it_cam){
    ++id;
    addButtonToPanel(varPanel, it_cam->first, id);
  }
  addSeparator();
	addColumn();
	varPanel= addPanel("Lights", 1);
  id=200;
  map<string, Light>::iterator it_light;
  int b[2]={0,1};
  for(it_light=&(((ANFScene*) scene)->lights.begin());it_light!=&(((ANFScene*) scene)->lights.end()); ++it_light){
    ++id;
    addCheckboxToPanel(varPanel, (char*)it_light->first.c_str(), b, id);
  }
  addSeparator();
	addColumn();
  id=300;
	varPanel= addPanel("Other", 1);
  GLUI_RadioGroup *drawing=addRadioGroup();
  addRadioButtonToGroup(drawing, "fill");
  addRadioButtonToGroup(drawing, "wireframe");
  addRadioButtonToGroup(drawing, "point");
  addRadioGroupToPanel(varPanel, drawing, 301);
  addSeparatorToPanel(varPanel);
  int range[10]={0,1,2,3,4,5,6,7,8,9};
  addSpinnerToPanel(varPanel, "Wind Speed", 2, range, 302);*/
}

void ANFInterface::processGUI(GLUI_Control *ctrl)
{
	switch (ctrl->user_id)
	{
		case 1:
		{
			break;
		};
		case 3:
		{
			break;
		};
	};
}
