#include "Model.h"

Model::Model()
{
	Translation[0]= 0;
	Translation[1] = 0;
	Translation[2] = 0;
	//zTranslation=0;
	//xRotation=0;
	//yRotation=0;
	rotation_matrix[0]= 1.0; //0.0, 0.0, 0.0,
    rotation_matrix[1]=0.0;    //                      0.0, 1.0, 0.0, 0.0,
    rotation_matrix[2]= 0.0;         //                0.0, 0.0, 1.0, 0.0,
    rotation_matrix[3]=0.0;
	rotation_matrix[4]=0.0;
	rotation_matrix[5]=1.0;
	rotation_matrix[6]=0.0;
	rotation_matrix[7]=0.0;
	rotation_matrix[8]=0.0;
	rotation_matrix[9]=0.0;
	rotation_matrix[10]=1.0;
	rotation_matrix[11]=0.0;
	rotation_matrix[12]=0.0;
	rotation_matrix[13]=0.0;
	rotation_matrix[14]=0.0;
	rotation_matrix[15]=1.0;
	//0.0, 0.0, 0.0, 1.0 };
	lightRotation=0;
	lightRotation2=0;

	light1Animation=true;
	light2Animation=true;
	visualizeNormals = false;

	light1.setAmbient( 0.0f, 0.0f, 0.0f, 1.0f );                 // Ambient Light Values ( NEW )
	light1.setDiffuse( 1.0f, 0.0f, 1.0f, 1.0f );              // Diffuse Light Values ( NEW )
	light1.setPosition(  0.0f, 0.0f, -2.0f );                 // Light Position ( NEW )

	glLightfv(GL_LIGHT0, GL_AMBIENT, light1.ambient);             // Setup The Ambient Light
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light1.diffuse);             // Setup The Diffuse Light
	glLightfv(GL_LIGHT0, GL_POSITION,light1.position);            // Position The Light

	light2.setAmbient( 0.0f, 0.0f, 0.0f, 1.0f );                 // Ambient Light Values ( NEW )
	light2.setDiffuse( 1.0f, 1.0f, 0.0f, 1.0f );              // Diffuse Light Values ( NEW )
	light2.setPosition(  0.0f, 0.0f, -2.0f );                 // Light Position ( NEW )

	glLightfv(GL_LIGHT1, GL_AMBIENT, light2.ambient);             // Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light2.diffuse);             // Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,light2.position);            // Position The Light


}

Model::~Model()
{
	delete [] indices;
	delete [] vertices;
}

void Model::Update()
{
	if(light1Animation)
		lightRotation+=0.3;
	if(light2Animation)
		lightRotation2+=0.6;
}
Vertex Model::getCenter()
{
	return center;
}

void Model::SetLight1Animation(bool value)
{
	light1Animation = value;
}
void Model::SetLight2Animation(bool value)
{
	light2Animation=value;
}
void Model::SetVisualizeNormals(bool value)
{
	visualizeNormals = value;
	if(visualizeNormals)
		glDisable(GL_LIGHTING);
	else
		glEnable(GL_LIGHTING);
}
void Model::Zoom(float zoomStep)
{
	zTranslation += zoomStep;
}
void Model::RotateX(float angleStep)
{
	xRotation+=angleStep;
}
void Model::RotateY(float angleStep)
{
	yRotation+=angleStep;
}
bool Model::LoadModel(char* filename)
{
	if(filename == NULL) return false;

	ifstream filestream(filename);
	
	if(!filestream.is_open())
		return false;
	while(filestream.good())
	{
		string line;
		getline(filestream,line);
		if(line.find("POINTS") != string::npos)
		{
			center.x=0;center.y=0;center.z=0;
			string strNumberOfPoints="";
			for(int i=0;i<line.length();i++)
			{
				if(isdigit(line[i])) //Make sure it is a number
					strNumberOfPoints+=line[i];
			}
			//Now we have the numberOfPoints forming the object
			numberOfPoints = atoi(strNumberOfPoints.c_str());
			//Initialize our Vertex Array
			vertices = new Vertex[numberOfPoints];
			//Loop on the numberOfPoints and fetch every point
			for(int i=0;i<numberOfPoints;i++)
			{
				filestream>>vertices[i].x>>vertices[i].y>>vertices[i].z;
				center.x=vertices[i].x;
				center.y=vertices[i].y;
				center.z=vertices[i].z;
			}
			center.x/=numberOfPoints;
			center.y/=numberOfPoints;
			center.z/=numberOfPoints;
		}
		//Find the Polygons Data
		if(line.find("POLYGONS") != string::npos)
		{
			string strNumberOfPolygons="";
			for(int i=0;i<line.length();i++)
			{
				if(line[i]==' ' && strNumberOfPolygons!="")
					break;
				if(isdigit(line[i])) //Make sure it is a number
					strNumberOfPolygons+=line[i];
			}
			//Now we have the numberOfPolygons forming the object
			numberOfPolygons = atoi(strNumberOfPolygons.c_str());
			//Initialize our Vertex Array
			indices = new int[numberOfPolygons*3];
			normals = new Vertex[numberOfPolygons];
			int numberOfSides;
			int index=0;
			//Loop on the numberOfPolygons and fetch the indices of each one
			for(int i=0;i<numberOfPolygons;i++)
			{
				filestream>>numberOfSides;
				filestream>>indices[index]>>indices[index+1]>>indices[index+2];
				//Now Calculate the Normal for this polygon and store it.
				float vector1[3];float vector2[3];
				vector1[0] = vertices[indices[index+1]].x - vertices[indices[index]].x;
				vector1[1] = vertices[indices[index+1]].y - vertices[indices[index]].y;
				vector1[2] = vertices[indices[index+1]].z - vertices[indices[index]].z;

				vector2[0] = vertices[indices[index+2]].x - vertices[indices[index]].x;
				vector2[1] = vertices[indices[index+2]].y - vertices[indices[index]].y;
				vector2[2] = vertices[indices[index+2]].z - vertices[indices[index]].z;

				//Now Get the cross product
				normals[i].x = vector1[1]*vector2[2] - vector2[1]*vector1[2];
				normals[i].y = vector1[2]*vector2[0] - vector2[2]*vector1[1];
				normals[i].z = vector1[0]*vector2[1] - vector2[0]*vector1[1];

				//Now we have to normalize that vector
				float length = sqrt(pow(normals[i].x,2) + pow(normals[i].y,2)+ pow(normals[i].z,2));

				normals[i].x/=length;
				normals[i].y/=length;
				normals[i].z/=length;

				vertices[indices[index]].nx +=normals[i].x;
				vertices[indices[index]].ny +=normals[i].y;
				vertices[indices[index]].nz +=normals[i].z;

				vertices[indices[index+1]].nx +=normals[i].x;
				vertices[indices[index+1]].ny +=normals[i].y;
				vertices[indices[index+1]].nz +=normals[i].z;

				vertices[indices[index+2]].nx +=normals[i].x;
				vertices[indices[index+2]].ny +=normals[i].y;
				vertices[indices[index+2]].nz +=normals[i].z;
				
				vertices[indices[index]].numberOffaces++;
				vertices[indices[index+1]].numberOffaces++;
				vertices[indices[index+2]].numberOffaces++;
				index+=3;
			}

			
			for(int i=0;i<numberOfPoints;i++)
			{
				vertices[i].nx/= vertices[i].numberOffaces;
				vertices[i].ny/= vertices[i].numberOffaces;
				vertices[i].nz/= vertices[i].numberOffaces;

				float length = sqrt(pow(vertices[i].nx,2) + pow(vertices[i].ny,2)+ pow(vertices[i].nz,2));
				vertices[i].nx/= length;
				vertices[i].ny/= length;
				vertices[i].nz/= length;
			}
		}
		//TEXTURE_COORDINATES
		if(line.find("TEXTURE_COORDINATES") != string::npos)
		{
			//Loop on the numberOfPoints and fetch every texture co-ordinate
			for(int i=0;i<numberOfPoints;i++)
				filestream>>vertices[i].u>>vertices[i].v;
			
		}
	}
	return true;
}

void Model::setTranslationXY(float x, float y)
{
	Translation[0]= x;
	Translation[1] = y;
}

void Model::setTranslationZ(float z)
{
	Translation[2]= z;
}
void Model::setRotation(float RotMatrix[16])
{
	for(int i=0;i<16;i++)
		rotation_matrix[i] = RotMatrix[i];
}
bool Model::getLight1Animation()
{
	return light1Animation;
}
bool Model::getLight2Animation()
{
	return light2Animation;
}
bool Model::LoadTexture(char* filename)
{	  
   GLubyte *texturedata; // The texture image
	int textureWidth;
	int textureHeight;
	// load texture
	ifstream textureFile;
	textureFile.open(filename, ios::in);
	if (textureFile.fail()){
	cout << "\n Error loading the texture";
	cout.flush(); exit(0);}
	string line;
	getline(textureFile,line); getline(textureFile,line);
	textureFile >> textureWidth;
	textureFile >> textureHeight;
	int maxRGBValue; textureFile >> maxRGBValue;
	texturedata = new GLubyte[textureWidth*textureHeight*4];

	int m,n,c;

	for(m=0;m<textureHeight;m++)
		for(n=0;n<textureWidth;n++)
		{
			textureFile >> c;
			texturedata[(m*textureWidth+n)*4]=(GLubyte) c;
			textureFile >> c;
			texturedata[(m*textureWidth+n)*4+1]=(GLubyte) c;
			textureFile >> c;
			texturedata[(m*textureWidth+n)*4+2]=(GLubyte) c;
			texturedata[(m*textureWidth+n)*4+3]=(GLubyte) 255;
		}
	textureFile.close();
	glGenTextures(1, &texture);

	//glBindTexture(GL_TEXTURE_2D, texture);

    //Set texture parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    //Define the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texturedata);

	delete[]texturedata;
   
	return true;
}

void Model::Render()
{	
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light1.diffuse);             // Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light2.diffuse);             // Setup The Diffuse Light
    // select our current texture
    //glBindTexture( GL_TEXTURE_2D, texture );
	glPushMatrix();

	glTranslatef(Translation[0], Translation[1],-Translation[2]) ;                      // Translate Into/Out Of The Screen By 

	glPushMatrix();
	glRotatef(lightRotation,1,0,0);

	glBegin(GL_POINT);
	glColor3f(light1.diffuse[0],light1.diffuse[1],light1.diffuse[2]);
	glVertex3f(0,0,-2);
	glEnd();

	light1.setPosition(0,0,-2);
	glLightfv(GL_LIGHT0, GL_POSITION,light1.position);            // Position The Light
	glPopMatrix();

	glPushMatrix();
	glRotatef(lightRotation2,0,1,0);

	glBegin(GL_POINT);
	glColor3f(light2.diffuse[0],light2.diffuse[1],light2.diffuse[2]);
	glVertex3f(0,0,-2);
	glEnd();
	light2.setPosition(0,0,-2);
	glLightfv(GL_LIGHT1, GL_POSITION,light2.position);            // 
	glPopMatrix();

	//glRotatef(xRotation,1.0f,0.0f,0.0f);                     // Rotate On The X Axis By xrot
	//glRotatef(yRotation,0.0f,1.0f,0.0f);                     // Rotate On The Y Axis By yrot
	//  Apply the rotation matrix
	glMultMatrixf (rotation_matrix);
	int index=0;
	glBegin(GL_TRIANGLES);
	for(int i=0;i<numberOfPolygons;i++)
	{
		glNormal3f(vertices[indices[index]].nx,vertices[indices[index]].ny,vertices[indices[index]].nz);
		glEnable(GL_NORMALIZE);

		glTexCoord2f(vertices[indices[index]].u, vertices[indices[index]].v);
		glVertex3f(vertices[indices[index]].x, vertices[indices[index]].y, vertices[indices[index]].z);
		
		glNormal3f(vertices[indices[index+1]].nx,vertices[indices[index+1]].ny,vertices[indices[index+1]].nz);
		glTexCoord2f(vertices[indices[index+1]].u, vertices[indices[index+1]].v);
		glVertex3f(vertices[indices[index+1]].x, vertices[indices[index+1]].y, vertices[indices[index+1]].z);

		glNormal3f(vertices[indices[index+2]].nx,vertices[indices[index+2]].ny,vertices[indices[index+2]].nz);
		glTexCoord2f(vertices[indices[index+2]].u, vertices[indices[index+2]].v);
		glVertex3f(vertices[indices[index+2]].x, vertices[indices[index+2]].y, vertices[indices[index+2]].z);	
		
		
		index+=3;
	}	
	glEnd();	

	if(visualizeNormals)
	{
		int index=0;
		glBegin(GL_LINES);
		for(int i=0;i<numberOfPoints;i++)
		{
			
			glColor3f(0,0,1);
			glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
			glVertex3f(vertices[i].x+vertices[i].nx * 0.01,vertices[i].y+vertices[i].ny * 0.01,vertices[i].z+vertices[i].nz * 0.01);
			
		}
		glEnd();
	}
	glPopMatrix();
	glFlush();
	
}
