#include "editor.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <rttr/registration.h>
#include "RenderManager.h"
#include "log.h"

#include <iostream>

const char* glsl_version = "#version 130";

Editor::Editor()
{

}

Editor::~Editor()
{

}

void Editor::init(GLFWwindow *window)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |=
		ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	io.ConfigFlags |=
		ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}

void Editor::startFrameRender()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Editor::endFrameRender()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void Editor::updateEditorGUI()
{
	ImGui::Begin("Node Editor");
    for (auto iter = RenderManager::getInstance().begin(); iter != RenderManager::getInstance().end(); ++iter) {
        auto nodeType = rttr::type::get(*(*iter));
        
        if (!nodeType.is_class()) {
            continue;
        }

    	ImGui::BulletText(nodeType.get_name().to_string().c_str());
		// LOGD("nodetpye: {}", nodeType.get_name().to_string())
        for (auto& method : nodeType.get_methods()) {
            // LOGD("******** property: {}", ).to_string());
			// ImGui::BulletText(prop.get_name().to_string().c_str());
            ImGui::BulletText(method.get_name().to_string().c_str());
			for (auto& para : method.get_parameter_infos()) {
            	ImGui::BulletText(para.get_name().to_string().c_str());
			}

        }

		
    }
	ImGui::End();
}

void Editor::destory()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
