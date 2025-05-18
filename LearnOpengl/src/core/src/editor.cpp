#include "editor.h"
#include "glm/ext/vector_float3.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <rttr/registration.h>
#include "RenderManager.h"
#include "log.h"
#include "rttr/variant.h"

#include <iostream>
#include <string>
#include <string_view>

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
        auto nodeType = rttr::type::get(*(iter->get()));
        
        if (!nodeType.is_class()) {
            continue;
        }

		ImGui::PushID(iter->get());
		if (ImGui::TreeNode(nodeType.get_name().to_string().c_str())) {
			const auto& methods = nodeType.get_methods();
			for (const auto& method : methods) {
				ImGui::PushID(method.get_name().to_string().c_str());
				
				// TODO: 对字符串前缀进行判断
				std::string methodName = method.get_name().to_string();
				if (methodName.substr(0,3) == "get") {
					ImGui::PopID();
					continue;
				}

				std::string functionName = methodName.substr(3);
				const auto& getMethod = nodeType.get_method("get"+functionName);

				ImGui::BulletText(functionName.c_str());

				size_t i = 0;
				for (const auto& property : method.get_parameter_infos()) {
					if (i > 0 && i < method.get_parameter_infos().size()-2) {
						ImGui::SameLine();
					}
					const auto propertyName = property.get_name().to_string();
					if (propertyName == "vec3") {
						rttr::variant ret = getMethod.invoke(*(iter->get()));
						if (!ret.is_valid() || !ret.is_type<glm::vec3>()) {
							LOGD("ret is unvalid or type is not vec3");
							continue;
						}
						glm::vec3 value = ret.get_value<glm::vec3>();

						if (ImGui::SliderFloat3(propertyName.c_str(), &value.x, -100.0f, 100.0f)) {
							method.invoke(*(iter->get()),value);
						}
					} else if (propertyName == "int") {
						rttr::variant ret = getMethod.invoke(*(iter->get()));
						if (!ret.is_valid() || !ret.is_type<int>()) {
							LOGD("ret is unvalid or type is not int");
							continue;
						}
						int value = ret.get_value<int>();

						if (ImGui::SliderInt(propertyName.c_str(), &value, -10, 10)) {
							method.invoke(*(iter->get()),value);
						}
					} else if (propertyName == "float") {
						rttr::variant ret = getMethod.invoke(*(iter->get()));
						if (!ret.is_valid() || !ret.is_type<float>()) {
							LOGD("ret is unvalid or type is not float");
							continue;
						}
						auto value = ret.get_value<float>();

						if (ImGui::SliderFloat(propertyName.c_str(), &value, -10.0f, 10.0f)) {
							method.invoke(*(iter->get()),value);
						}
					} else if (propertyName == "bool") {
						rttr::variant ret = getMethod.invoke(*(iter->get()));
						if (!ret.is_valid() || !ret.is_type<bool>()) {
							LOGD("ret is unvalid or type is not float");
							continue;
						}
						auto value = ret.get_value<bool>();

						if (ImGui::Checkbox(propertyName.c_str(), &value)) {
							method.invoke(*(iter->get()),value);
						}
					}

					i++;
				}	

				ImGui::PopID();
			}

			ImGui::TreePop();
		}

		ImGui::PopID();

    }
	ImGui::End();
}

void Editor::destory()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
