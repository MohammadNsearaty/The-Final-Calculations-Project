// dear imgui: standalone example application for GLUT/FreeGLUT + OpenGL2, using legacy fixed pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.

// !!! GLUT/FreeGLUT IS OBSOLETE SOFTWARE. Using GLUT is not recommended unless you really miss the 90's. !!!
// !!! If someone or something is teaching you GLUT in 2019, you are being abused. Please show some resistance. !!!

#include "imgui.h"
#include "imgui_impl_glut.h"
#include "imgui_impl_opengl2.h"
#include<iostream>
#include<math.h>
#include"Cube.h"
#include"Shpere.h"
#include"PhysicsEngine.h"
#include"BoundingBox.h"
#include<time.h>
#include<vector>
#include"Ray.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <freeglut.h>
#endif

#ifdef _MSC_VER
#pragma warning (disable: 4505) // unreferenced local function has been removed
#endif

static bool show_demo_window = true;
static bool show_another_window = false;
static ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.00f, 1.00f);
void reshape(int w, int h);
void timer(int t);
GLUquadric *q = gluNewQuadric();
double movX = 0.0f, movY = 0.0f, movZ = 0.0f;
double lX, lY;
float mass,mass1;
int arm,raduis;
float color[3],color1[3];

float v[3] = { 0.0f,0.0f,0.0f };
float v1[3] = { 0.0f,0.0f,0.0f };
//the physics engine
PhysicsEngine engine = PhysicsEngine();

bool addCube = false;
bool addSphere = false;
int frame = 0, Time, timeBase = 0;
float fps = 600.0f;
void camera();


vec3 testForce = vec3(0.0007,0,0);
vec3 virtualGravity = vec3(0, -0.98,0);
vec3 tVec = vec3(0, -0.1, 0);
bool checkbox1_enabled = false;
bool checkbox2_enabled = false;
Cube newCube;
Shpere newShpere;
float mv = 0.001;
//int res = 0;
float dist = 0.0;
void Gui();
void Gui()
{
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		//if (show_demo_window)
	//	ImGui::ShowDemoWindow(&show_demo_window);


	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	static float f = 0.0f;
	static int counter = 0;
	ImGuiIO& io = ImGui::GetIO();
/*	if (io.MouseDown[0] || io.MouseDown[1])
	{
		system("CLS");
		ImVec2 mouse = io.MousePos;
		float h = glutGet(GLUT_WINDOW_HEIGHT);
		float w = glutGet(GLUT_WINDOW_WIDTH);
		float x = (mouse.x - (w / 2));
		float y = (mouse.y - (h / 2));
		x /= 120 /2;
		y /= 72 / 2;
		
		Ray ray = Ray(vec3(movX, movY, movZ), vec3(x, y, 1));
		cout << ray.orgin.x << "," << ray.orgin.y << "," << ray.orgin.z << endl;
		cout<<engine.rayCast(cube1, ray);
	}
	*/
	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.


	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
					
    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
	if (io.KeysDown[111])/*ImGui::Button("O") to Close Game Mode*/
		glutLeaveGameMode();
	if ( io.KeysDown[119] /*ImGui::Button("  W  ")*/)
		movY += mv;
	if (io.KeysDown[115]  /*ImGui::Button("  S  ")*/)
		movY -= mv;
	if (io.KeysDown[97]  /*ImGui::Button("  A  ")*/)
		movX -= mv;
	if (io.KeysDown[100] /*ImGui::Button("  D  ")*/)
		movX += mv;

	if (io.KeysDown[357]/*ImGui::Button("  Up  ")*/)
		lY += mv;
	if (io.KeysDown[359]/*ImGui::Button("  Down ")*/)
		lY -= mv;
	if (io.KeysDown[356]/*ImGui::Button("  Left  ")*/)
		lX += mv;
	if (io.KeysDown[358]/*ImGui::Button("  Right  ")*/)
		lX -= mv;
	if (io.KeysDown[120]/*the X key*/)
		movZ -= mv;
	if(io.KeysDown[122]/*the Z key*/)
		movZ += mv;
	if (ImGui::Button("add cube"))
	{
		addCube = true;
	}
	if (addCube)
	{
		ImGui::InputFloat3("Cube Position", &v[0], 2);
		ImGui::InputFloat("Cube Mass", &mass);
		ImGui::InputInt("Cube Rib length", &arm);
		ImGui::ColorEdit3("Cube Color", &color[0]);
		ImGui::Checkbox("wire draw", &checkbox2_enabled);
		if (ImGui::Button("Submit"))
		{
			newCube = Cube(vec3(v[0], v[1], v[2]), mass, arm, vec3(color[0],color[1],color[2]));
			if (checkbox2_enabled)
				newCube.setDrawType(2);
			checkbox2_enabled = false;
			engine.addCube(newCube);
			addCube = false;
		}
	}
	if (ImGui::Button("add Sphere"))
	{
		addSphere = true;
	}
	if (addSphere)
	{
		ImGui::InputFloat3("Sphere Position", &v1[0]);
		ImGui::InputFloat("Sphere Mass", &mass1);
		ImGui::InputInt("Sphere Raduis", &raduis);
		ImGui::ColorEdit3("Sphere Color", &color1[0]);
		ImGui::Checkbox("wire draw", &checkbox1_enabled);
		if (ImGui::Button("Submit"))
		{
			newShpere = Shpere(vec3(v1[0],v1[1],v1[2]),mass1,raduis,vec3(color1[0],color1[1],color1[2]));
			if (checkbox1_enabled)
				newShpere.setDrawType(2);
			checkbox1_enabled = false;
			engine.addShper(newShpere);
			addSphere = false;
		}
	}
	ImGui::SliderFloat("camera speed", &mv, 0.0f, 1.0f);   
	ImGui::Text("Application average %.3f ms/frame (%.d FPS)", 1000.0f / (ImGui::GetIO().Framerate), fps);
	ImGui::End();

	}
void my_display_code()
{
	glPushMatrix();
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BITS);
		glScaled(0.1, 0.1, 0.1);
		float dur = 1 / fps;
		engine.drawCubes();
		engine.drawShperes();
	}
	glPopMatrix();
}
void glut_display_func()
{

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGLUT_NewFrame();
	camera();
	my_display_code();

	Gui();
	// Rendering
	ImGui::Render();
	ImGuiIO& io = ImGui::GetIO();
	glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());


	glutSwapBuffers();
	glutPostRedisplay();

	frame++;
	Time = glutGet(GLUT_ELAPSED_TIME);
	if (Time - timeBase > 1000)
	{
		fps = frame * 1000 / (Time - timeBase);
		timeBase = Time;
		frame = 0;
	}
	
}


int main(int argc, char** argv)
{
	// Create GLUT window
	glutInit(&argc, argv);
#ifdef __FREEGLUT_EXT_H__
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
#endif
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	glutInitWindowSize(1200, 720);
	glutCreateWindow("Dear ImGui GLUT+OpenGL2 Example");
	glClear(GL_COLOR_BUFFER_BIT);
/*	if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))
	{
	//	glutGameModeString("16");
		glutEnterGameMode();
	}
	*/
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glutDisplayFunc(glut_display_func);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
	glutIdleFunc(glut_display_func);
	
	//// Setup Dear ImGui context
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGLUT_Init();
	ImGui_ImplGLUT_InstallFuncs();
	ImGui_ImplOpenGL2_Init();



	//scale the cordinates
	glScaled(0.1, 0.1, 0.1);	
	glEnable(GL_DEPTH_TEST);
	
	glutMainLoop();

	//Cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();

	return 0;
}
void reshape(int w, int h)
{

	// Compute aspect ratio of the new window
	if (h == 0) h = 1;                // To prevent divide by 0
	GLfloat aspect = (GLfloat)w / (GLfloat)h;

	// Set the viewport to cover the new window
	glViewport(0, 0, w, h);

	// Set the aspect ratio of the clipping volume to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             // Reset
								  // Enable perspective projection with fovy, aspect, zNear and zFar
	gluPerspective(45.0f, aspect, 0.1f, 100.0f);

}
void timer(int)
{
	glutPostRedisplay();
	glutTimerFunc(1, timer, 0);
}
void camera() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(movX, movY, movZ, lX, lY, 1, 0, 1, 0);
}
