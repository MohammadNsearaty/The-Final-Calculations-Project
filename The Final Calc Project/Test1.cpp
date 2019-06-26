#include "imgui.h"
#include "imgui_impl_glut.h"
#include "imgui_impl_opengl2.h"
class test
{
	
public:
	void excute();
};

void test::excute()
{
	ImGui::Begin("test");
	if (ImGui::Button("add cube"));
	{

	}

}