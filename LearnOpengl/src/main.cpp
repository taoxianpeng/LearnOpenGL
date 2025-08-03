#include "application.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION

#include "common.h"
#include "log.h"
#include "camera.h"
#include "editor.h"
#include "direction.h"
#include "RenderManager.h"
#include "keyboard.h"
#include "imgui.h"

using namespace std;

#if defined (_WIN32) || defined (_WIN64)
	#define WIN
#elif defined (__linux__) 
	#define LINUX
#endif

extern Application* createApplication();
Application* application = createApplication();

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool firstMouse = true;
bool isDragging = false;

void processInput(GLFWwindow* window);

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadCubeMap(const vector<string>& faces);

static void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


int main(int argc, char** argv) {

#ifdef WIN 
	Direction::getInstance()->loadConfig("config_win.json");
#elif defined (LINUX)
	Direction::getInstance()->loadConfig("config_linux.json");
#endif

	init_logger();

	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit()) return 1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	GLFWwindow* window = glfwCreateWindow(
		1280, 720, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
	if (window == nullptr) return 1;

	// 帧缓冲时调整视口大小
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetCursorPosCallback(window, mouse_callback);

	// glfwSetMouseButtonCallback(window, mouse_pressed_callback);

	glfwSetScrollCallback(window, scroll_callback);

	glfwMakeContextCurrent(window);

	gladLoadGL();
	glfwSwapInterval(1);  // Enable vsync

	Editor editor;
	editor.init(window);

	application->initializeGL();
	application->onProjectLoad();

	RenderManager::getInstance().loadResource();

	//// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	//unsigned int rbo;
	//glGenRenderbuffers(1, &rbo);
	//glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it

	// if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	// 	spdlog::error("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

	// glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 帧缓冲 End




	// bool show_demo_window = false;
	// bool show_another_window = false;
	// glm::vec4 clear_color(0.45f, 0.55f, 0.60f, 1.00f);


	// // light
	// static Light light_material;
	// light_material.ambient = glm::vec3(1.0f, 0.5f, 0.31f);
	// light_material.diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
	// light_material.speculer = glm::vec3(1.0f, 0.5f, 0.31f);
	// light_material.position = glm::vec3(1.2f, 1.0f, 2.0f);

	// // material
	// static Material object_material;
	// object_material.ambient = glm::vec3(1.0f, 0.5f, 0.31f);
	// object_material.diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
	// object_material.speculer = glm::vec3(1.0f, 0.5f, 0.31f);
	// object_material.shininess = 32.0f;

	// static glm::vec3 light_position = glm::vec3(1.0f, 1.0f, 1.0f);

	// 加载纹理
	// Texture quartTex;

	// quartTex.create("C:\\Users\\txp\\source\\repos\\LearnOpengl\\LearnOpengl\\src\\assert\\blending_transparent_window.PNG");
	// vertPath = resourcePath + "/shader/window.vert";
	// fragPath = resourcePath + "/shader/window.frag";
	// Shader quartShader(vertPath, fragPath);


	CheckCall(glEnable(GL_DEPTH_TEST));
	CheckCall(glEnable(GL_BLEND));

	// 开启混合
	CheckCall(glEnable(GL_BLEND));
	CheckCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	while (!glfwWindowShouldClose(window)) {
		
		processInput(window);
		glfwPollEvents();

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		editor.startFrameRender();

		// if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

		// // 3. Show another simple window.
		// if (show_another_window) {
		// 	ImGui::Begin(
		// 		"Another Window",
		// 		&show_another_window);  // Pass a pointer to our bool variable (the
		// 	// window will have a closing button that will
		// 	// clear the bool when clicked)
		// 	ImGui::Text("Hello from another window!");
		// 	if (ImGui::Button("Close Me")) show_another_window = false;
		// 	ImGui::End();
		// }

		// Rendering
		CheckCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
		CheckCall(glClear(
			GL_COLOR_BUFFER_BIT |
			GL_DEPTH_BUFFER_BIT |
			GL_STENCIL_BUFFER_BIT
		));

		application->onUpdate();

		// create transformations
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(
			glm::radians(camera.Zoom),
			static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f,
			1000.0f);
		
		RenderManager::getInstance().setPV(projection, view);
		editor.updateEditorGUI();
		RenderManager::getInstance().drawAll();

		// {
		// 	static float f = 0.0f;
		// 	static int counter = 0;

		// 	ImGui::Begin("Hello, world!");  // Create a window called "Hello, world!"
		// 	// and append into it.

		// 	ImGui::Text("Color_1");
		// 	// ImGui::InputFloat("X", (float*)&light_position.x);
		// 	// ImGui::InputFloat("Y", (float*)&light_position.y);
		// 	// ImGui::InputFloat("Z", (float*)&light_position.z);
		// 	ImGui::SliderFloat("X", &light_position.x, -10.0f, 10.0f);
		// 	ImGui::SliderFloat("Y", &light_position.y, -10.0f, 10.0f);
		// 	ImGui::SliderFloat("Z", &light_position.z, -10.0f, 10.0f);
		// 	ImGui::ColorEdit3("light ambient", (float*)&light_material.ambient);
		// 	ImGui::ColorEdit3("light diffuse", (float*)&light_material.diffuse);
		// 	ImGui::ColorEdit3("light specular", (float*)&light_material.speculer);

		// 	ImGui::Spacing();
		// 	ImGui::ColorEdit3("object ambient", (float*)&object_material.ambient);
		// 	ImGui::ColorEdit3("object diffuse", (float*)&object_material.diffuse);
		// 	ImGui::ColorEdit3("object specular", (float*)&object_material.speculer);
		// 	// shininess
		// 	ImGui::SetNextItemWidth(150);
		// 	ImGui::SliderFloat("shininess", &object_material.shininess, 0.0f, 128.0f);

		// 	ImGui::Spacing();
		// 	ImGui::Text("lastX = %f", lastX);
		// 	ImGui::Text("lastY = %f", lastY);

		// 	ImGui::Text(
		// 		"camera front = (%f,%f,%f) | up = (%f,%f,%f) | right = (%f,%f,%f)",
		// 		camera.Front.x, camera.Front.y, camera.Front.z, camera.Up.x,
		// 		camera.Up.y, camera.Up.z, camera.Position.x, camera.Position.y,
		// 		camera.Position.z);

		// 	ImGui::End();
		// }	


		editor.endFrameRender();
		// Render
		glfwSwapBuffers(window);
	}

	editor.destory();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);

	KeyActionEvent actionEvent;
	for (auto keyItem = static_cast<int>(KeyEvent::KEY_SPACE); keyItem < static_cast<int>(KeyEvent::KEY_MAX); ++keyItem) {
		actionEvent = static_cast<KeyActionEvent>(glfwGetKey(window, keyItem));
		if (actionEvent == KeyActionEvent::PRESS) {
			application->onKeyInputEvent(static_cast<KeyEvent>(keyItem), KeyActionEvent::PRESS);	
		} else if (actionEvent == KeyActionEvent::RELEASE) {
			// application->onKeyInputEvent(static_cast<KeyEvent>(keyItem), KeyActionEvent::RELEASE);	
		} else if (actionEvent == KeyActionEvent::REPEAT) {
			application->onKeyInputEvent(static_cast<KeyEvent>(keyItem), KeyActionEvent::REPEAT);	
		}
	}

}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	// Check if ImGui wants to capture the mouse
	if (ImGui::GetIO().WantCaptureMouse) {
		return; // Do not process mouse movement if ImGui is handling it
	}

	// Check if left mouse button is pressed
	bool leftMouseButtonPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

	if (leftMouseButtonPressed) {
		if (!isDragging) {
			// Store the current mouse position as the last position for dragging
			lastX = xpos;
			lastY = ypos;
			isDragging = true;
		}
		else {
			float xoffset = xpos - lastX;
			float yoffset = lastY - ypos;  // reversed since y-coordinates go from bottom to top

			lastX = xpos;
			lastY = ypos;

			camera.ProcessMouseMovement(xoffset, yoffset);
		}
	}
	else {
		isDragging = false;
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadCubeMap(const vector<string>& faces)
{
	GLuint textureID;
	CheckCall(glGenTextures(1, &textureID));
	CheckCall(glBindBuffer(GL_TEXTURE_CUBE_MAP, textureID));

	int width, height, nrChannels;
	for (auto i = 0; i < faces.size(); ++i) {
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			CheckCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, static_cast<void*>(data)));
		}
		else {
			spdlog::error("Cubmap texture failed to load at path:{}", faces[i]);
		}
		stbi_image_free(data);
	}

	CheckCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	CheckCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	CheckCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	CheckCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
	CheckCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	return textureID;

}

