#include "ANFInterface.h"

void ANFInterface::processKeyboard(unsigned char key, int x, int y)
{
	// Uncomment below if you would like to process the default keys (e.g. 's' for snapshot, 'Esc' for exiting, ...)
	CGFinterface::processKeyboard(key, x, y);
}

void ANFInterface::initGUI(){
/*	int id=0;
	// Check CGFinterface.h and GLUI documentation for the types of controls available
	GLUI_Panel *optionsPanel= addPanel("Options", 1);
	addColumnToPanel(optionsPanel);
	GLUI_Panel *lightList=addPanelToPanel(optionsPanel, "Lights", 1);
	map<string, Light>::iterator it_light;
	for(it_light=scene->lights.begin();it_light!=scene->lights.end();++it_light){
		if(it_light->second.enabled){
			addCheckboxToPanel(lightList,it_light->first, id)->set_int_val(1);
		}else{
			addCheckboxToPanel(lightList,it_light->first, id)->set_int_val(0);
		}
		++id;
	}

	addColumnToPanel(optionsPanel);
	GLUI_Panel *camPanel=addPanelToPanel(optionsPanel, "Cameras", 1);
	GLUI_RadioGroup *camList=addRadioGroupToPanel(camPanel, scene->activeCam);
	map<string, Camera*>::iterator it_cam;
	for(it_cam=scene->cameras.begin();it_cam=scene->cameras.end();++it_cam){
		if(it_cam->first==currentCam){
			addRadioButtonToGroup(camList, it_cam->first)->set_int_value(1);
		}else{
			addRadioButtonToGroup(camList, it_cam->first)->set_int_value(0);
		}
	}
	addRadioButtonToGroup(camList, "Default")->set_int_val(0);
/*
	addColumnToPanel(optionsPanel);
	GLUI_Panel *Misc=addPanelToPanel(optionsPanel, "Drawing", 1);
  GLUI_RadioGroup *drawing=addRadioGroupToPanel(Misc);
  addRadioButtonToGroup(drawing, "fill");
  addRadioButtonToGroup(drawing, "wireframe");
  addRadioButtonToGroup(drawing, "point");
	addColumnToPanel(Misc);
	GLUI_Panel *windPanel = addPanelToPanel(Misc, "Wind", 3);
	GLUI_Spinner *wind = addSpinnerToPanel(windPanel, "Wind", 2, &scene->wind, id);*/
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
