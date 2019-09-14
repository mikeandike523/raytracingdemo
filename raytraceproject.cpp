#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include <iostream>
#include <GLShader.hpp>
#include <Dense>
#include <cmath>
//note bounce
//reflected = incident - 2(incident dot N)*N
//rule basic shadows, occlusion si the same, but if bounced ray does not intersect a light plane, then it is in shadow
using namespace Eigen;
// This will identify our vertex buffer
const float PI = 3.1415926535897932384626433832795;
const int A = 0, S = 1, W = 2, D = 3;
GLuint vertexbuffer;
GLint locWindowWidth;
GLint locWindowHeight;
GLint baseRuler;
GLint numTRsLoc;
GLint pposLoc;
GLint pangLoc;
GLint worldtex;
GLint normaltex;
GLuint textureID;
GLuint textureID2;
Vector3f ppos(0,0,0);

//yaw xz (upright = PI/2), pitch zy (upright =0), roll xy (upright =0)
Vector3f pang(PI/2,0,0);
int keys[] = { 0,0,0,0,0,0 }; //a s w d left right


void changeSize(int w, int h) {
	glUniform1f(locWindowWidth, (float)w);
	glUniform1f(locWindowHeight, (float)h);
	glViewport(0, 0, w, h);
}
void renderScene(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
	
	



	glutPostRedisplay();
	glutSwapBuffers();
}

void NormalKeyHandler(unsigned char key, int x, int y)
{
	if (key == 'a')
		keys[0] = 1;
	else if (key == 's')
		keys[1] = 1;
	else if (key == 'w')
		keys[2] = 1;
	else if (key == 'd')
		keys[3] = 1;
}

void SpecialKeyHandler(int key, int x, int y)
{
	if(key == GLUT_KEY_RIGHT)
		keys[5] = 1;
	if (key == GLUT_KEY_LEFT)
		keys[4] = 1;
}
void NormalKeyUpHandler(unsigned char key, int x, int y)
{
	if (key == 'a')
		keys[0] = 0;
	else if (key == 's')
		keys[1] = 0;
	else if (key=='w')
		keys[2] = 0;
	else if (key=='d')
		keys[3] = 0;
}

void SpecialKeyUpHandler(int key, int x, int y)
{
	if (key == GLUT_KEY_RIGHT)
		keys[5] = 0;
	if (key == GLUT_KEY_LEFT)
		keys[4] = 0;
	// and so on...
}

void movePlayer() {

	if (keys[4] == 1) {
		pang(0) += PI / 100;
	}
	if (keys[5] == 1) {
		pang(0) -= PI / 100;
	}
	if (keys[0] == 1 || keys[1] == 1 || keys[2] == 1 || keys[3] == 1) {
		float ang = pang(0); //yaw
		Vector3f moveVector(0, 0, 0);
		//aswd
		if (keys[W] == 1) {
			moveVector = Vector3f(cos(ang), 0, sin(ang));
		}
		if (keys[A] == 1) {
			moveVector = Vector3f(-sin(ang), 0, cos(ang));
		}
		if (keys[S] == 1) {
			moveVector = Vector3f(-cos(ang), 0, -sin(ang));
		}
		if (keys[D] == 1) {
			moveVector = Vector3f(sin(ang), 0, -cos(ang));
		}
		ppos += 3.0f*moveVector;
	
	}
}
void timer(int)
{

	movePlayer();
	glUniform3f(pposLoc,ppos(0),ppos(1),ppos(2));
	glUniform3f(pangLoc, pang(0), pang(1), pang(2));
	glUniform1f(baseRuler, 1.0f);
	glutPostRedisplay();
	glutTimerFunc(1000.0 / 60.0, timer, 0);
}
int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(-1, -1);
	glutInitWindowSize(500, 500);
	glutCreateWindow("OpenGL First Window");

	glewInit();
	if (glewIsSupported("GL_VERSION_4_5"))
	{
		std::cout << " GLEW Version is 4.5\n ";
	}
	else
	{
		std::cout << "GLEW 4.5 not supported\n ";
	}

	glEnable(GL_DEPTH_TEST);
	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glViewport(0, 0, 500, 500);
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	static const GLfloat g_vertex_buffer_data[] = {
		 
		  -250.0f, 250.0f, 0.0f,
		-250.0f, -250.0f, 0.0f,
		   250.0f,  250.0f, 0.0f,
		   250.0f,-250.0f,0.0f
	}; 



	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0);
	
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint program = LoadShader(R"(D:\raytracerproj\raytraceproject\raytraceproject\shader.vert)", R"(D:\raytracerproj\raytraceproject\raytraceproject\shader.frag)");
	locWindowWidth = glGetUniformLocation(program,"windowWidth");
	locWindowHeight = glGetUniformLocation(program, "windowHeight");
	numTRsLoc = glGetUniformLocation(program, "numTRs");
	pposLoc= glGetUniformLocation(program, "ppos");
	pangLoc = glGetUniformLocation(program, "pang");
	normaltex = glGetUniformLocation(program, "normaltex");
	worldtex = glGetUniformLocation(program, "worldtex");
	std::cout << "aaa "<<normaltex<<std::endl;
	glUseProgram(program);



	glUniform1i(normaltex, 1);
	glUniform1i(worldtex, 0);
	glUniform1f(locWindowWidth, 500.0f);
	glUniform1f(locWindowHeight, 500.0f);
	glUniform1f(baseRuler, 1.0f); //set to initial window size, base ruler can be used as a zoom factor
	glActiveTexture(GL_TEXTURE0);

	/*texture test*/

	glGenTextures(1, &textureID);

	glGenTextures(1, &textureID2);
	
	glActiveTexture(GL_TEXTURE0);
	
	glBindTexture(GL_TEXTURE_2D, textureID);
	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	const float dist = 200.0f;
	const float wallAspect = 1.0f / 8.0f;
	GLfloat pixels[] = {
		 //north wall tr1
		-dist,dist*wallAspect,dist,    -dist,-dist*wallAspect,dist,      dist,dist*wallAspect,dist,

		//north wall tr2
		dist,dist*wallAspect,dist,    dist,-dist * wallAspect,dist,      -dist,-dist*wallAspect,dist,



		//south wall tr1
	   -dist,dist*wallAspect,-dist,    -dist,-dist * wallAspect,-dist,      dist,dist*wallAspect,-dist,

	   //south wall tr2
	   dist,dist*wallAspect,-dist,    dist,-dist * wallAspect,-dist,      -dist,-dist * wallAspect,-dist,

	   //east wall tr1
	   dist,dist*wallAspect,dist,     dist,-dist*wallAspect,dist,     dist,dist*wallAspect,-dist,

	   //east wall tr2
	   dist,-dist*wallAspect,-dist,     dist,-dist * wallAspect,dist,     dist,dist*wallAspect,-dist,

	   //west wall tr1
	-dist,dist*wallAspect,dist,     -dist,-dist * wallAspect,dist,     -dist,dist*wallAspect,-dist,

	//west wall tr2
	-dist,-dist * wallAspect,-dist,     -dist,-dist * wallAspect,dist,     -dist,dist*wallAspect,-dist,

	};



	glUniform1i(numTRsLoc, sizeof(pixels)/(sizeof(GLfloat)*3));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, sizeof(pixels)/(sizeof(GLfloat)*3), 1, 0, GL_RGB, GL_FLOAT, pixels);


	GLfloat normals[sizeof(pixels) / (sizeof(GLfloat)*3)];
	for (int i = 0; i < sizeof(pixels) / (sizeof(GLfloat) * 9); i++) {
		Vector3f PA(pixels[i * 9 + 0], pixels[i * 9 + 1], pixels[i * 9 + 2]);
		Vector3f PB(pixels[i * 9 + 3], pixels[i * 9 + 4], pixels[i * 9 + 5]);
		Vector3f PC(pixels[i * 9 + 6], pixels[i * 9 + 7], pixels[i * 9 + 8]);
		Vector3f N = (PB - PA).cross(PC - PA).normalized();
		
		normals[i * 3 + 0] = N(0);
		normals[i * 3 + 1] = N(1);
		normals[i * 3 + 2] = N(2);
		//std::cout << N(0) << " " << N(1) << " " << N(2) << std::endl;
	}
	for(int i=0;i<sizeof(normals)/sizeof(GLfloat);i++)
	std::cout << (float)normals[i]<<std::endl;

	

	


	glActiveTexture(GL_TEXTURE1);
	
	glBindTexture(GL_TEXTURE_2D, textureID2);
	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, sizeof(normals) / (sizeof(GLfloat) * 3), 1, 0, GL_RGB, GL_FLOAT, normals);



	//success
	glutTimerFunc(1000.0 / 60.0, timer, 0);
	glutSpecialFunc(SpecialKeyHandler);
	glutKeyboardFunc(NormalKeyHandler);
	glutSpecialUpFunc(SpecialKeyUpHandler);
	glutKeyboardUpFunc(NormalKeyUpHandler);
	glutMainLoop();

	return 0;
}