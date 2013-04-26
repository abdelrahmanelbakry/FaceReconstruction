//#include <stdlib.h>
//#include <fstream>

#include <iostream>
#include<gl\glui.h>
#include "Model.h"



using namespace std;
void OnResize (int newWidth,int newHeight);
void OnFrameMove();
void OnFrameRender();
void keyboard(unsigned char key, int x, int y);
void initGL();
void initUI();

Model* model;
//UI Variables
int winHandle;
int visNormals = 0;
int firstLight=1;
int secondLight=1;
int wireframe=0;
int textureEnable=1;

//  Translation XY Live Variable
float translate_xy[2] = {0, 0};
//  Translation Z Live Variable
float translate_z = 0;
//  Rotation Matrix Live Variable Array (Initialize as Identity Matrix)
float rotation_matrix[16] = { 1.0, 0.0, 0.0, 0.0,
                              0.0, 1.0, 0.0, 0.0,
                              0.0, 0.0, 1.0, 0.0,
                              0.0, 0.0, 0.0, 1.0 };

float RedComp = 150;
float GreenComp = 150;
float BlueComp = 150;

int selectedLight=0;

int animationEnable=1;

GLUI_Spinner *redCompSpinner;
GLUI_Spinner *greenCompSpinner;
GLUI_Spinner *blueCompSpinner;
GLUI_Checkbox* LightanimationEnable;

#define TRANSLATION_XY 13
#define TRANSLATION_Z 14
#define ROTATION 15
/**************************************** control_cb() *******************/
/* GLUI control callback                                                 */

void control_cb( int control )
{
	if(control==1)
	{
	  if(visNormals==0)
		  model->SetVisualizeNormals(false);
	  else
		  model->SetVisualizeNormals(true);
	}
	if(control==2)
	{
		if(firstLight ==0) glDisable(GL_LIGHT0);
		else glEnable(GL_LIGHT0);
	}
	if(control==3)
	{
		if(secondLight ==0) glDisable(GL_LIGHT1);
		else glEnable(GL_LIGHT1);
	}
	if(control==4)
	{
		if(selectedLight == 0)
		{
			redCompSpinner->set_float_val(model->light1.diffuse[0]);
			greenCompSpinner->set_float_val(model->light1.diffuse[1]);
			blueCompSpinner->set_float_val(model->light1.diffuse[2]);
			LightanimationEnable->set_int_val(model->getLight1Animation());
		}
		else
		{
			redCompSpinner->set_float_val(model->light2.diffuse[0]);
			greenCompSpinner->set_float_val(model->light2.diffuse[1]);
			blueCompSpinner->set_float_val(model->light2.diffuse[2]);
			LightanimationEnable->set_int_val(model->getLight2Animation());
		}
	}
	if(control == 5)
	{
		if(selectedLight == 0)
			model->light1.setRedDiffuse(RedComp);
		else
			model->light2.setRedDiffuse(RedComp);
	}
	if(control == 6)
	{
		if(selectedLight == 0)
			model->light1.setGreenDiffuse(GreenComp);
		else
			model->light2.setGreenDiffuse(GreenComp);
	}
	if(control == 7)
	{
		if(selectedLight == 0)
			model->light1.setBlueDiffuse(BlueComp);
		else
			model->light2.setBlueDiffuse(BlueComp);
	}
	if(control == 8)
	{
		if(selectedLight == 0)
			model->SetLight1Animation(animationEnable);
		else
			model->SetLight2Animation(animationEnable);
	}
	if(control==9)
	{
		exit(1);
	}
	if(control==10)
	{
		if(wireframe)
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		else
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}
	if(control==11)
	{
		if(textureEnable)
			glEnable(GL_TEXTURE_2D);
		else
			glDisable(GL_TEXTURE_2D);
	}
	if(control == TRANSLATION_XY)
		model->setTranslationXY(translate_xy[0],translate_xy[1]);
	if(control == TRANSLATION_Z)
		model->setTranslationZ(translate_z);
	if(control == ROTATION)
		model->setRotation(rotation_matrix);
}

int main(int argc, char** argv)
{
  // Initialize graphics window first
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (800, 600); 
  glutInitWindowPosition (0, 0);
  winHandle=glutCreateWindow ("Face Reconstruction");

  // Initialize OpenGL
  initGL();

  initUI();
  // Initialize callback functions
  glutDisplayFunc(OnFrameRender); 
 // glutIdleFunc(OnFrameMove);
  glutReshapeFunc(OnResize);
  glutKeyboardFunc(keyboard);


  // Start rendering 
  glutMainLoop();
}
void initUI()
{
  /****************************************/
  /*         Here's the GLUI code         */
  /****************************************/
  GLUI *glui = GLUI_Master.create_glui( "Properties" );
  GLUI_Panel *enable_panel = glui->add_panel (
    "Enables");
  glui->add_checkbox_to_panel(enable_panel,  "Visualize Normals", &visNormals,1,control_cb );
  glui->add_checkbox_to_panel(enable_panel, "Light #1 Enable", &firstLight,2,control_cb );
  glui->add_checkbox_to_panel(enable_panel, "Light #2 Enable", &secondLight,3,control_cb  );
  glui->add_checkbox_to_panel(enable_panel, "Wireframe", &wireframe,10,control_cb  );
  glui->add_checkbox_to_panel(enable_panel, "Texture", &textureEnable,11,control_cb  );

  //  Add a separator
  glui->add_separator();
  //(new GLUI_Spinner( glui, "Segments:", &segments ))
   // ->set_int_limits( 3, 60 ); 
   GLUI_Panel *light_panel = glui->add_panel (
    "Light Properties");
   GLUI_Listbox*selectLightBox= glui->add_listbox_to_panel(light_panel,"Select Light",&selectedLight,4,control_cb);
   selectLightBox->add_item(0,"Light #1");
   selectLightBox->add_item(1,"Light #2");
   selectLightBox->set_int_val(0);

	//  Add the Red Component spinner
	redCompSpinner = glui->add_spinner_to_panel (
		light_panel, "Red", GLUI_SPINNER_FLOAT,
		&RedComp, 5, control_cb);
	//  Set the limits for the spinner
	redCompSpinner->set_float_limits ( 0, 1.0 );
	redCompSpinner->set_float_val(model->light1.diffuse[0]);

	//  Add the Red Component spinner
	greenCompSpinner = glui->add_spinner_to_panel (
		light_panel, "Green", GLUI_SPINNER_FLOAT,
		&GreenComp, 6, control_cb);
	//  Set the limits for the spinner
	greenCompSpinner->set_float_limits ( 0, 1.0 );
	greenCompSpinner->set_float_val(model->light1.diffuse[1]);

	//  Add the Red Component spinner
	blueCompSpinner = glui->add_spinner_to_panel (
		light_panel, "Blue", GLUI_SPINNER_FLOAT,
		&BlueComp, 7, control_cb);
	//  Set the limits for the spinner
	blueCompSpinner->set_float_limits ( 0, 1.0 );
	blueCompSpinner->set_float_val(model->light1.diffuse[2]);
	
	//Add a check box to enable/disable light animation
	LightanimationEnable = glui->add_checkbox_to_panel(light_panel,  "Animation On/Off", &animationEnable,8,control_cb );

	// Add the 'Transformation' Panel to the GLUI window
	GLUI_Panel *transformation_panel = glui->add_panel (
    "Transformation");
	GLUI_Panel *translation_panel = glui->add_panel_to_panel (
    transformation_panel, "");

	//  Add the xy translation control
	GLUI_Translation *translation_xy = glui->add_translation_to_panel(
		translation_panel, "Translation XY",
		GLUI_TRANSLATION_XY, translate_xy,
		TRANSLATION_XY, control_cb );
	//  Set the translation speed
	translation_xy->set_speed( 0.005 );
	//  Add column, but don't draw it
	glui->add_column_to_panel (translation_panel, false);

	//  Add the z translation control
	GLUI_Translation *translation_z = glui->add_translation_to_panel (
		translation_panel, "Translation Z",
		GLUI_TRANSLATION_Z, &translate_z,
		TRANSLATION_Z, control_cb );
	//  Set the translation speed
	translation_z->set_speed( 0.005 );

	//  Create transformation panel 2 that will contain the rotation and
	//  spinner controls
	GLUI_Panel *rotation_panel = glui->add_panel_to_panel (
		transformation_panel, "");
	//  Add the rotation control
	glui->add_rotation_to_panel (rotation_panel,
		"Rotation", rotation_matrix, ROTATION, control_cb);

	//  Add a separator
	glui->add_separator();
	glui->add_button("Quit",9,control_cb);

   glui->set_main_gfx_window( winHandle );

  /* We register the idle callback with GLUI, *not* with GLUT */
  GLUI_Master.set_glutIdleFunc( OnFrameMove ); 
}
void initGL()
{
//	GLfloat LightAmbient[]= { 0.0f, 0.0f, 0.0f, 1.0f };                 // Ambient Light Values ( NEW )
//	GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };              // Diffuse Light Values ( NEW )
//	GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };                 // Light Position ( NEW )

	glEnable( GL_LIGHTING );
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	glEnable(GL_TEXTURE_2D);                        // Enable Texture Mapping ( NEW )
	
    glShadeModel(GL_SMOOTH);                        // Enable Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);                   // Black Background
    glClearDepth(1.0f);                         // Depth Buffer Setup
	
    glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing
    //glDepthFunc(GL_LEQUAL);                         // The Type Of Depth Testing To Do
 //   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);          // Really Nice Perspective 

	model = new Model();
	model->LoadModel("face.vtk.txt");
	model->LoadTexture("face.ppm");

	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glViewport (0, 0, 800, 600); 
	//gluPerspective(45.0f,800.0f/600.0f,0.1f,500.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); //CT <- I
	gluLookAt(0,0,-1.0,model->getCenter().x, model->getCenter().y, model->getCenter().z, 0, 1, 0);		
}

void OnResize (int newWidth,int newHeight)
{
    cout << "reshape" << endl;
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);          // Really Nice Perspective 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport (0, 0, (GLsizei) newWidth, (GLsizei) newHeight); 
	double asratio;

    if (newHeight == 0) newHeight = 1; //to avoid divide-by-zero

    asratio = newWidth / (double) newHeight;

	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);          // Really Nice Perspective 
    gluPerspective(45.0f,asratio,0.1f,500.0f);

	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity(); //CT <- I
//	gluLookAt(0,0,-1.0,model->getCenter().x, model->getCenter().y, model->getCenter().z, 0, 1, 0);

}
void OnFrameMove ()
{
	if ( glutGetWindow() != winHandle ) 
		glutSetWindow(winHandle);  
  model->Update();
  glutPostRedisplay();
	
}

void keyboard(unsigned char key, int x, int y)
{
	/*
	float step = 0.5;
	switch(toupper(key))
	{
		case 'W':
			model->Zoom(-1*step);
			break;
		case 'S':
			model->Zoom(step);
			break;
		case 'D':
			model->RotateY(step);
			break;
		case 'A':
			model->RotateY(-1*step);
			break;
		case 'I':
			model->RotateX(step);
			break;
		case 'K':
			model->RotateX(-1*step);
			break;
	}
	*/
}



void OnFrameRender()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
  model->Render();


  glFlush();

  glutSwapBuffers();
  glutPostRedisplay();
}
