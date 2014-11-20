#include "ANFScene.h"
using namespace std;

ANFScene::ANFScene(char *filename) {
	//prepare path
	char path[128];
	strcat(path, "../data/");
	//prepare file
	char file[128];
	strcpy(file, path);
	strcat(file, filename);
	//read file
	doc = new TiXmlDocument(file);

	if (!doc->LoadFile()) {
		printf("Could not load file '%s'. Error='%s'. Exiting.\n", file,
		doc->ErrorDesc());
		exit(1);
	}

	TiXmlElement* anfElement = doc->FirstChildElement("anf");

	if (anfElement == NULL) {
		printf("Main ANF block element not found! Exiting!\n");
		exit(1);
	}
	printf("Main ANF block element found! Processing...\n");

	globalsElement = anfElement->FirstChildElement("globals");
	camerasElement = anfElement->FirstChildElement("cameras");
	lightsElement = anfElement->FirstChildElement("lights");
	texturesElement = anfElement->FirstChildElement("textures");
	appearancesElement = anfElement->FirstChildElement("appearances");
	animationsElement = anfElement->FirstChildElement("animations");
	graphElement = anfElement->FirstChildElement("graph");

	//globals section
	if (globalsElement) {

		TiXmlElement *drawElement = globalsElement->FirstChildElement(
		"drawing");

		globals.drawing.mode = drawElement->Attribute("mode");
		globals.drawing.shading = drawElement->Attribute("shading");
		sscanf(drawElement->Attribute("background"), "%f %f %f %f",
		&globals.drawing.background[0], &globals.drawing.background[1],
		&globals.drawing.background[2], &globals.drawing.background[3]);

		TiXmlElement *cullingElement = globalsElement->FirstChildElement(
		"culling");

		globals.culling.face = cullingElement->Attribute("face");
		globals.culling.order = cullingElement->Attribute("order");

		TiXmlElement *lightingElement = globalsElement->FirstChildElement(
		"lighting");

		lightingElement->QueryBoolAttribute("doublesided",
		&globals.lighting.dsided);
		lightingElement->QueryBoolAttribute("local", &globals.lighting.local);
		lightingElement->QueryBoolAttribute("enabled",
		&globals.lighting.enabled);
		sscanf(lightingElement->Attribute("ambient"), "%f %f %f %f",
		&globals.lighting.ambient[0], &globals.lighting.ambient[1],
		&globals.lighting.ambient[2], &globals.lighting.ambient[3]);

		printf("Globals block processed!\n");
	} else {
		printf("Globals block not found!\n");
	}

	//cameras section
	if (camerasElement) {
		initialCam = camerasElement->Attribute("initial");

		TiXmlElement *pCamElem = camerasElement->FirstChildElement(
		"perspective");
		CamPerspective* camP;
		while (pCamElem) {
			camP=new CamPerspective();
			camP->id = pCamElem->Attribute("id");
			pCamElem->QueryFloatAttribute("near", &camP->near);
			pCamElem->QueryFloatAttribute("far", &camP->far);
			pCamElem->QueryFloatAttribute("angle", &camP->angle);
			sscanf(pCamElem->Attribute("pos"), "%f %f %f", &camP->pos[0],
			&camP->pos[1], &camP->pos[2]);
			sscanf(pCamElem->Attribute("target"), "%f %f %f", &camP->target[0],
			&camP->target[1], &camP->target[2]);

			cameras[camP->id] = camP;

			pCamElem = pCamElem->NextSiblingElement("perspective");
		}

		TiXmlElement *oCamElem = camerasElement->FirstChildElement("ortho");
		CamOrtho* camO;
		while (oCamElem) {
			camO = new CamOrtho();
			camO->id = oCamElem->Attribute("id");
			oCamElem->QueryFloatAttribute("near", &camO->near);
			oCamElem->QueryFloatAttribute("far", &camO->far);
			oCamElem->QueryFloatAttribute("left", &camO->left);
			oCamElem->QueryFloatAttribute("right", &camO->right);
			oCamElem->QueryFloatAttribute("top", &camO->top);
			oCamElem->QueryFloatAttribute("bottom", &camO->bottom);

			cameras[camO->id] = camO;

			oCamElem = oCamElem->NextSiblingElement("ortho");
		}

		printf("Cameras block processed!\n");
	} else {
		printf("Cameras block not found!\n");
	}

	//lights section
	if (lightsElement) {
		TiXmlElement *light = lightsElement->FirstChildElement();
		TiXmlElement *lightComponent;
		Light* l;
		while (light) {
			l= new Light();
			l->id = light->Attribute("id");
			l->type = light->Attribute("type");
			light->QueryBoolAttribute("enabled", &l->enabled);
			light->QueryBoolAttribute("marker", &l->marker);
			sscanf(light->Attribute("pos"), "%f %f %f", &l->pos[0], &l->pos[1], &l->pos[2]);
			if (l->type=="spot"){
				sscanf(light->Attribute("target"), "%f %f %f", &l->target[0], &l->target[1], &l->target[2]);
				light->QueryFloatAttribute("angle", &l->angle);
				light->QueryFloatAttribute("exponent", &l->exponent);
			}
			lightComponent = light->FirstChildElement();
			while (lightComponent) {
				string type = lightComponent->Attribute("type");
				if ( type == "ambient") {
					sscanf(lightComponent->Attribute("value"), "%f %f %f %f",
						&l->component[0][0], &l->component[0][1], &l->component[0][2],
						&l->component[0][3]);
			} else if ( type == "diffuse") {
				sscanf(lightComponent->Attribute("value"), "%f %f %f %f",
					&l->component[1][0], &l->component[1][1], &l->component[1][2],
					&l->component[1][3]);
			} else if (type == "specular") {
				sscanf(lightComponent->Attribute("value"), "%f %f %f %f",
					&l->component[2][0], &l->component[2][1], &l->component[2][2],
					&l->component[2][3]);
				}
				lightComponent = lightComponent->NextSiblingElement();
			}
			lights[l->id] = l;
			light = light->NextSiblingElement();
		}
		printf("Lights block processed!\n");
	} else {
		printf("Lights block not found!\n");
	}

	//textures section
	if (texturesElement) {
		TiXmlElement *texture = texturesElement->FirstChildElement();
		Texture t;
		string file_name;
		while (texture) {
			t.id = texture->Attribute("id");
			file_name=texture->Attribute("file");
			t.file = "../data/"+file_name;
			texture->QueryFloatAttribute("texlength_s", &t.length_s);
			texture->QueryFloatAttribute("texlength_t", &t.length_t);

			textures[t.id] = t;

			texture = texture->NextSiblingElement();
		}
		printf("Textures block processed!\n");
	} else {
		printf("Textures block not found!\n");
	}

	//appearances section
	if (appearancesElement) {
		TiXmlElement *appearance = appearancesElement->FirstChildElement();
		TiXmlElement *appearanceComponent;
		Appearance app;
		float shininess, ambient[4], difuse[4], specular[4];
		string component;
		while (appearance) {
			app.id = appearance->Attribute("id");
			appearance->QueryFloatAttribute("shininess", &shininess);
			if (appearance->Attribute("textureref")) {
				app.textureref = appearance->Attribute("textureref");
			} else {
				app.textureref = "";
			}
			appearanceComponent = appearance->FirstChildElement();
			while (appearanceComponent) {
				component = appearanceComponent->Attribute("type");
				if (component == "ambient") {
					sscanf(appearanceComponent->Attribute("value"),
					"%f %f %f %f", &ambient[0], &ambient[1],
					&ambient[2], &ambient[3]);
				} else if (component == "diffuse") {
					sscanf(appearanceComponent->Attribute("value"),
					"%f %f %f %f", &difuse[0], &difuse[1],
					&difuse[2], &difuse[3]);
				} else if (component == "specular") {
					sscanf(appearanceComponent->Attribute("value"),
					"%f %f %f %f", &specular[0], &specular[1],
					&specular[2], &specular[3]);
				}
				appearanceComponent = appearanceComponent->NextSiblingElement();
			}
			app.ap = new CGFappearance(ambient, specular, difuse, shininess);
			if(app.textureref!=""){
				app.ap->setTexture(textures[app.textureref].file);
			}
			appearances[app.id] = app;
			appearance = appearance->NextSiblingElement();
		}
		printf("Appearances block processed!\n");
	} else {
		printf("Appearances block not found!\n");
	}

	// animations section
	if(animationsElement) {
		TiXmlElement *animation = animationsElement->FirstChildElement();
		TiXmlElement *control;
		string type;
		LinearAnimation* la;
		CircularAnimation* ca;
		while(animation){
			type=animation->Attribute("type");
			if(type=="linear"){
				la=new LinearAnimation();
				la->id=animation->Attribute("id");
				animation->QueryFloatAttribute("span", &la->span);
				control=animationsElement->FirstChildElement();
				float c;
				vector<float> vc;
				while(control){
					control->QueryFloatAttribute("xx", &c);
					vc.push_back(c);
					control->QueryFloatAttribute("yy", &c);
					vc.push_back(c);
					control->QueryFloatAttribute("zz", &c);
					vc.push_back(c);
					la->controlPoint.push_back(vc);
					vc.clear();
					control=control->NextSiblingElement();
				}
				animations[la->id]=la;
			}else{
				ca=new CircularAnimation();
				ca->id=animation->Attribute("id");
				animation->QueryFloatAttribute("span", &ca->span);
				sscanf(animation->Attribute("center"), "%f %f %f", &ca->center[0],
				&ca->center[1], &ca->center[2]);
				animation->QueryFloatAttribute("radius", &ca->radius);
				animation->QueryFloatAttribute("startang", &ca->startang);
				animation->QueryFloatAttribute("rotang", &ca->rotang);
				animations[ca->id]=ca;
			}
			animation=animation->NextSiblingElement();
		}
		printf("Animations block processed!\n");
	} else {
		printf("Animations block not found!\n");
	}

	// graph section
	if (graphElement) {
		graph.rootid = graphElement->Attribute("rootid");
		TiXmlElement *nodeElement = graphElement->FirstChildElement();
		Node* node;
		string type;
		if (nodeElement) {
			TiXmlElement *transforms;
			TiXmlElement *appearance;
			TiXmlElement *animations;
			TiXmlElement *primitives;
			while (nodeElement) {
				node = new Node();
				node->id = nodeElement->Attribute("id");

				transforms =nodeElement->FirstChildElement("transforms");
				glLoadIdentity();
				glGetFloatv(GL_MODELVIEW_MATRIX,node->matrix);
				if(transforms){
					transforms=transforms->FirstChildElement();
					while (transforms) {
						type = transforms->Attribute("type");
						if (type == "translate") {
							float t[3];
							sscanf(transforms->Attribute("to"), "%f %f %f", &t[0],
							&t[1], &t[2]);
							glTranslatef(t[0], t[1], t[2]);
						} else if (type == "rotate") {
							float angle;
							string axis;
							axis = transforms->Attribute("axis");
							transforms->QueryFloatAttribute("angle", &angle);
							glRotatef(angle, axis == "x", axis == "y", axis == "z");
						} else {
							float s[3];
							sscanf(transforms->Attribute("factor"), "%f %f %f",
							&s[0], &s[1], &s[2]);
							glScalef(s[0], s[1], s[2]);
						}
						glGetFloatv(GL_MODELVIEW_MATRIX, node->matrix);
						transforms = transforms->NextSiblingElement();
					}
				}

				appearance = nodeElement->FirstChildElement("appearanceref");
				if (appearance) {
					node->appearanceref = appearance->Attribute("id");
				} else {
					node->appearanceref = "";
				}

				animations =nodeElement->FirstChildElement("animationref");
				while(animations){
					node->animationref.push_back(animations->Attribute("id"));
					animations=animations->NextSiblingElement("animationref");
				}

				primitives = nodeElement->FirstChildElement("primitives");
				if (primitives) {
					TiXmlElement *triangles = primitives->FirstChildElement(
					"triangle");
					while (triangles) {
						Triangle* t = new Triangle();
						sscanf(triangles->Attribute("xyz1"), "%f %f %f",
						&t->xyz1[0], &t->xyz1[1], &t->xyz1[2]);
						sscanf(triangles->Attribute("xyz2"), "%f %f %f",
						&t->xyz2[0], &t->xyz2[1], &t->xyz2[2]);
						sscanf(triangles->Attribute("xyz3"), "%f %f %f",
						&t->xyz3[0], &t->xyz3[1], &t->xyz3[2]);

						t->calcNormal();

						node->primitives.push_back(t);
						triangles = triangles->NextSiblingElement("triangle");
					}

					TiXmlElement *rectangles = primitives->FirstChildElement(
					"rectangle");
					while (rectangles) {
						Rectangle* r = new Rectangle();
						sscanf(rectangles->Attribute("xy1"), "%f %f",
						&r->xy1[0], &r->xy1[1]);
						sscanf(rectangles->Attribute("xy2"), "%f %f",
						&r->xy2[0], &r->xy2[1]);
						node->primitives.push_back(r);
						rectangles = rectangles->NextSiblingElement(
						"rectangles");
					}

					TiXmlElement *cylinders = primitives->FirstChildElement(
					"cylinder");
					while (cylinders) {
						Cylinder* c = new Cylinder();
						cylinders->QueryFloatAttribute("base", &c->base);
						cylinders->QueryFloatAttribute("top", &c->top);
						cylinders->QueryFloatAttribute("height", &c->height);
						cylinders->QueryIntAttribute("slices", &c->slices);
						cylinders->QueryIntAttribute("stacks", &c->stacks);

						node->primitives.push_back(c);
						cylinders = cylinders->NextSiblingElement("cylinder");
					}

					TiXmlElement *spheres = primitives->FirstChildElement(
					"sphere");
					while (spheres) {
						Sphere* s = new Sphere();
						spheres->QueryFloatAttribute("radius", &s->radius);
						spheres->QueryIntAttribute("slices", &s->slices);
						spheres->QueryIntAttribute("stacks", &s->stacks);

						node->primitives.push_back(s);
						spheres = spheres->NextSiblingElement("sphere");
					}

					TiXmlElement *torus = primitives->FirstChildElement(
					"torus");
					while (torus) {
						Torus* t = new Torus();
						torus->QueryFloatAttribute("inner", &t->inner);
						torus->QueryFloatAttribute("outer", &t->outer);
						torus->QueryIntAttribute("slices", &t->slices);
						torus->QueryIntAttribute("loops", &t->loops);

						node->primitives.push_back(t);
						torus = torus->NextSiblingElement("torus");
					}
					TiXmlElement *planes = primitives->FirstChildElement("plane");
					while (planes){
						Plane* p = new Plane();
						planes->QueryIntAttribute("parts", &p->parts);
						node->primitives.push_back(p);
						planes = planes->NextSiblingElement("plane");
					}

					TiXmlElement *patches = primitives->FirstChildElement("patch");
					while(patches){
						Patch* p = new Patch();

						patches->QueryIntAttribute("order", &p->order);
						patches->QueryIntAttribute("partsU", &p->partsU);
						patches->QueryIntAttribute("partsV", &p->partsV);
						p->compute=patches->Attribute("compute");
						int i=0;
						TiXmlElement *c = patches->FirstChildElement();
						while(c){
							sscanf(c->Attribute("x"), "%f",
							&p->controlPoint[i][0]);
							sscanf(c->Attribute("y"), "%f",
							&p->controlPoint[i][1]);
							sscanf(c->Attribute("z"), "%f",
							&p->controlPoint[i][2]);
							++i;
							c=c->NextSiblingElement();
						}
						//Calculate Texture Coords
						//TODO check this (patch texture coord)
						for(int j=0; j<i/2; ++j){
							p->texturePoint[j][0]=0.0;
							p->texturePoint[j][1]=0.0+1/i*j;
							p->texturePoint[j+i/2][0]=1.0;
							p->texturePoint[j+i/2][1]=0.0+1/i*j;
						}
						node->primitives.push_back(p);
						patches = patches->NextSiblingElement("patch");
					}

				}

				TiXmlElement *descendants = nodeElement->FirstChildElement(
				"descendants")->FirstChildElement();
				while (descendants) {
					node->descendants.push_back(descendants->Attribute("id"));
					descendants = descendants->NextSiblingElement();
				}

				graph.nodes[node->id] = node;
				nodeElement = nodeElement->NextSiblingElement();
			}
		} else {
			printf("Node block not found!\n");
		}
		printf("Graph block processed!\n");
	} else {
		printf("Graph block not found!\n");
	}
}

void ANFScene::init() {
	if (globals.lighting.enabled){
		glEnable(GL_LIGHTING);
	}else{
		glDisable(GL_LIGHTING);
	}

	if (globals.lighting.dsided){
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	}else{
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	}

	if (globals.lighting.local){
		glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	}else{
		glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
	}

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globals.lighting.ambient);

	if (globals.drawing.mode == "gouraud"){
		glShadeModel(GL_SMOOTH);
	}else if (globals.drawing.mode == "flat"){
		glShadeModel(GL_FLAT);
	}

	if (globals.drawing.mode == "fill"){
		glPolygonMode(GL_FRONT, GL_FILL);
	}else if (globals.drawing.mode == "line"){
		glPolygonMode(GL_FRONT, GL_LINE);
	}else if (globals.drawing.mode == "point"){
		glPolygonMode(GL_FRONT, GL_POINT);
	}
	glClearColor(globals.drawing.background[0], globals.drawing.background[1],
	globals.drawing.background[2], globals.drawing.background[3]);

	glEnable(GL_CULL_FACE);
	if (globals.culling.face == "none"){
		glCullFace(GL_NONE);
	}else if (globals.culling.face == "front"){
		glCullFace(GL_FRONT);
	}else if (globals.culling.face == "back"){
		glCullFace(GL_BACK);
	}else if (globals.culling.face == "both"){
		glCullFace(GL_FRONT_AND_BACK);
	}
	if (globals.culling.order == "cw"){
		glFrontFace(GL_CW);
	}else if (globals.culling.order == "ccw"){
		glFrontFace(GL_CCW);
	}

	//init lights
	map<string, Light*>::iterator it;
	for(it=lights.begin();it!=lights.end();++it){
		unsigned int id=0;
		it->second->light = new CGFlight(id, it->second->pos);
		if(it->second->type=="spot"){
			glLightfv(id, GL_SPOT_DIRECTION, it->second->target);
			glLightf(id, GL_SPOT_CUTOFF, it->second->angle);
			glLightf(id, GL_SPOT_EXPONENT, it->second->exponent);
			it->second->light->setAmbient(it->second->component[0]);
			it->second->light->setDiffuse(it->second->component[1]);
			it->second->light->setSpecular(it->second->component[2]);
		}
		++id;
	}

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_2D);

	unsigned int updatePeriod=30;

	setUpdatePeriod(updatePeriod);
}

void ANFScene::update(unsigned long t){
	map<string, Animation*>::iterator it_anim;
	for(it_anim=animations.begin();it_anim!=animations.end(); ++it_anim){
		it_anim->second->update(t);
	}
}

void ANFScene::display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//cameras[initialCam]->apply();
	CGFapplication::activeApp->forceRefresh();
	CGFscene::activeCamera->applyView();

	map<string, Light*>::iterator it;
	for(it=lights.begin(); it!=lights.end();++it){

		if(it->second->marker){
			it->second->light->draw();
		}
			it->second->light->disable();
		if(it->second->enabled){
			it->second->light->enable();
		}
		it->second->light->update();
	}
	axis.draw();
	//-------------------------------
	drawNode(graph.nodes[graph.rootid]);

	glutSwapBuffers();
}

void ANFScene::drawNode(Node* n) {
	glPushMatrix();

	if ((n->appearanceref != "inherit") && (n->appearanceref != "")){
		appearances[n->appearanceref].ap->apply();
	}
	glMultMatrixf(n->matrix);
	for(size_t i=0; i<n->animationref.size(); ++i){
		animations[n->animationref[i]]->apply();
	}
	for (size_t i = 0; i < n->primitives.size(); ++i) {
		glPushMatrix();
		n->primitives[i]->draw();
		glPopMatrix();
	}
	for (size_t j = 0; j < n->descendants.size(); ++j) {
		drawNode(graph.nodes[n->descendants[j]]);
	}
	glPopMatrix();
}

ANFScene::~ANFScene() {
	//TODO complete this
	delete (doc);
	delete (globalsElement);
	delete (camerasElement);
	delete (lightsElement);
	delete (texturesElement);
	delete (appearancesElement);
	delete (animationsElement);
	delete (graphElement);
}
