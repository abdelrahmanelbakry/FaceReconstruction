#ifndef _MODEL_H_
#define _MODEL_H_

#include<fstream>
#include<iostream>
#include<string>
#include <GL/glut.h>


using namespace std;
struct Light
{
	float ambient[4];
	float specular[4];
	float diffuse[4];
	float position[4];

	void setPosition(float x, float y, float z)
	{
		position[0] = x;
		position[1] = y;
		position[2]=z;
		position[3] = 1.0;
	}
	void setRedDiffuse(float R)
	{
		diffuse[0]=R;
	}
	void setGreenDiffuse(float G)
	{
		diffuse[1]=G;
	}
	void setBlueDiffuse(float B)
	{
		diffuse[2]=B;
	}
	void setAmbient(float R,float G,float B, float A)
	{
		ambient[0] = R; ambient[1] = G; ambient[2] = B; ambient[3] = A;
	}
	void setDiffuse(float R,float G,float B, float A)
	{
		diffuse[0] = R; diffuse[1] = G; diffuse[2] = B; diffuse[3] = A;
	}
	void setSpecular(float R,float G,float B, float A)
	{
		specular[0] = R; specular[1] = G; specular[2] = B; specular[3] = A;
	}
};
struct Vertex
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat u; GLfloat v;
	GLfloat nx,ny,nz;
	GLint numberOffaces;
	Vertex(GLfloat X, GLfloat Y,GLfloat Z)
	{
		x=X;y=Y;z=Z;
		nx=0;ny=0,nz=0;
		numberOffaces=0;
	}
	Vertex()
	{
		x=0;y=0;z=0; u=0,v=0; 
		nx=0;ny=0,nz=0;
		numberOffaces=0;
	}
};
class Model
{
	private:
		Vertex center;
		Vertex* vertices;
		int* indices;
		Vertex* normals;

		

		float lightRotation;
		float lightRotation2;

		float xRotation;
		float yRotation;
		float zTranslation;

		int numberOfPoints;
		int numberOfPolygons;
		 GLuint texture;

		 bool visualizeNormals;
		 bool light1Animation;
		 bool light2Animation;
	public:

		Model();
		~Model();

		Light light1;
		Light light2;

		float Translation[3];
		float rotation_matrix[16];
		/*= { 1.0, 0.0, 0.0, 0.0,
                              0.0, 1.0, 0.0, 0.0,
                              0.0, 0.0, 1.0, 0.0,
                              0.0, 0.0, 0.0, 1.0 };*/
		
		void setTranslationXY(float x, float y);
		void setTranslationZ(float z);
		void setRotation(float RotMatrix[16]);
		bool getLight1Animation();
		bool getLight2Animation();
		void SetLight1Animation(bool value);
		void SetLight2Animation(bool value);
		void SetVisualizeNormals(bool value);
		void Update();
		bool LoadModel(char* filename);
		bool LoadTexture(char* filename);
		void Render();
		Vertex* getVertices();
		Vertex getCenter();


		void Zoom(float zoomStep);
		void RotateX(float angleStep);
		void RotateY(float angleStep);
};

#endif