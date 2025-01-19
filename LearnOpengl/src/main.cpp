
#include <iostream>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <checkError.h>

#define STB_IMAGE_IMPLEMENTATION

#include "camera.h"
#include "common.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "mesh.h"
#include "texture.h"

#include "big_man_model.h"

using namespace std;


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

void framebufferWindow(GLuint textureID) {
	ImGui::Begin("Preview window");
	float scale_factor = std::min((float)ImGui::GetContentRegionAvail().x / 800.0f, (float)ImGui::GetContentRegionAvail().y / 600.0f);
	ImGui::Image((ImTextureID)(intptr_t)textureID, ImVec2(800 * scale_factor, 600 * scale_factor), ImVec2(1, 1), ImVec2(0, 0));
	ImGui::End();
}

int main(int argc, char** argv) {
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit()) return 1;

	const char* glsl_version = "#version 130";
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

	//std::filesystem::path shaderDir =
	//	filesystem::path("C:\\Users\\txp\\source\\repos\\LearnOpengl\\LearnOpengl\\src") / "shader";
	//std::filesystem::path modelDir =
	//	filesystem::path("C:\\Users\\txp\\source\\repos\\LearnOpengl\\LearnOpengl\\src") / "assert";

	//std::string vertPath = (shaderDir / "shader.vert").string();
	//std::string fragPath = (shaderDir / "shader.frag").string();
	//std::string modelPath = (modelDir / "nanosuit/nanosuit.obj").string();

	std::string resourcePath = "C:/Users/txp/source/repos/LearnOpengl/LearnOpengl/src";
	//std::string vertPath = resourcePath + "/shader/shader.vert";
	//std::string fragPath = resourcePath + "/shader/shader.frag";
	//std::string modelPath = resourcePath + "/assert/nanosuit/nanosuit.obj";

	std::string axioPath = resourcePath + "/assert/axio/axio.obj";

	//spdlog::info("vertPath:{}", vertPath);
	//spdlog::info("fragPath:{}", fragPath);
	//spdlog::info("modelPath:{}", modelPath);
	//spdlog::info("AxioPath:{}", axioPath);

	//Shader ourShader(vertPath, fragPath);
	//MMesh::Model ourModel(modelPath);

	BigManModel bigManModel;
	
	bigManModel.loadData();

	//模型边缘绘制
	std::string shaderSingleColorFragPath = resourcePath + "/shader/singleColor.frag";

	// sun
	std::string vertPath = resourcePath + "/shader/sun.vert";
	std::string fragPath = resourcePath + "/shader/sun.frag";
	std::string modelPath = resourcePath + "/assert/sun/sun.obj";

	Shader sunShader(vertPath, fragPath);
	MMesh::Model sunModel(modelPath);

	Shader singleColorShader(vertPath, shaderSingleColorFragPath);

	// axio
	vertPath = resourcePath + "/shader/axio.vert";
	fragPath = resourcePath + "/shader/axio.frag";
	Shader axioShader(vertPath, fragPath);
	MMesh::Model axioModel(axioPath);


	/* 构建窗户 Start */
	float verties[] = {
		-0.5f, -0.5, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	std::vector<glm::vec3> windows
	{
		glm::vec3(-1.5f, 0.0f, -0.48f),
		glm::vec3(1.5f, 0.0f, 0.51f),
		glm::vec3(0.0f, 0.0f, 0.7f),
		glm::vec3(-0.3f, 0.0f, -2.3f),
		glm::vec3(0.5f, 0.0f, -0.6f)
	};


	GLuint VAO = 0, VBO = 0, EBO = 0;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verties), verties, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	/* 构建窗户 End */



	//// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	//unsigned int rbo;
	//glGenRenderbuffers(1, &rbo);
	//glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		spdlog::error("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 帧缓冲 End


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

	bool show_demo_window = false;
	bool show_another_window = false;
	glm::vec4 clear_color(0.45f, 0.55f, 0.60f, 1.00f);


	// light
	static Light light_material;
	light_material.ambient = glm::vec3(1.0f, 0.5f, 0.31f);
	light_material.diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
	light_material.speculer = glm::vec3(1.0f, 0.5f, 0.31f);
	light_material.position = glm::vec3(1.2f, 1.0f, 2.0f);

	// material
	static Material object_material;
	object_material.ambient = glm::vec3(1.0f, 0.5f, 0.31f);
	object_material.diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
	object_material.speculer = glm::vec3(1.0f, 0.5f, 0.31f);
	object_material.shininess = 32.0f;

	static glm::vec3 light_position = glm::vec3(1.0f, 1.0f, 1.0f);

	// 加载纹理
	Texture quartTex;

	quartTex.create("C:\\Users\\txp\\source\\repos\\LearnOpengl\\LearnOpengl\\src\\assert\\blending_transparent_window.PNG");
	vertPath = resourcePath + "/shader/window.vert";
	fragPath = resourcePath + "/shader/window.frag";
	Shader quartShader(vertPath, fragPath);


	while (!glfwWindowShouldClose(window)) {
		
		processInput(window);
		glfwPollEvents();

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

		// 3. Show another simple window.
		if (show_another_window) {
			ImGui::Begin(
				"Another Window",
				&show_another_window);  // Pass a pointer to our bool variable (the
			// window will have a closing button that will
			// clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me")) show_another_window = false;
			ImGui::End();
		}

		// Rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(
			GL_COLOR_BUFFER_BIT |
			GL_DEPTH_BUFFER_BIT |
			GL_STENCIL_BUFFER_BIT
		);


		// create transformations
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(
			glm::radians(camera.Zoom),
			static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f,
			100.0f);
		glm::mat4 model = glm::mat4(1.0f);


		/*ourShader.use();
		ourShader.setMat4("view", view);
		ourShader.setMat4("projection", projection);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		ourShader.setMat4("model", model);
		ourShader.setVec3("light.position", light_position);
		ourShader.setVec3("light.diffuse", light_material.diffuse);
		ourShader.setVec3("viewPos", camera.Position);
		ourShader.setFloat("light.constant", 1.0f);
		ourShader.setFloat("light.linear", 0.09f);
		ourShader.setFloat("light.quadratic", 0.032f);*/
		bigManModel.setView(view);
		bigManModel.setModel(model);
		bigManModel.setProjection(projection);
		bigManModel.setCamera(camera);
		bigManModel.setLightMaterial(light_material);
		bigManModel.setLightPosition(light_position);

		bigManModel.draw();
		// sun
		sunShader.use();
		sunShader.setMat4("view", view);
		sunShader.setMat4("projection", projection);
		model = glm::translate(model, light_position);
		sunShader.setMat4("model", model);

		sunModel.draw(sunShader);

		float scale = 1.1f;
		singleColorShader.use();
		singleColorShader.setMat4("view", view);
		singleColorShader.setMat4("projection", projection);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		singleColorShader.setMat4("model", model);

		sunModel.draw(singleColorShader);

		// axio
		axioShader.use();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		axioShader.setMat4("view", view);
		axioShader.setMat4("projection", projection);
		axioShader.setMat4("model", model);
		axioModel.draw(axioShader);


		glBindVertexArray(0);
		// 2. Show a simple window that we create ourselves. We use a Begin/End pair
		// to create a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");  // Create a window called "Hello, world!"
			// and append into it.

			ImGui::Text("Color_1");
			// ImGui::InputFloat("X", (float*)&light_position.x);
			// ImGui::InputFloat("Y", (float*)&light_position.y);
			// ImGui::InputFloat("Z", (float*)&light_position.z);
			ImGui::SliderFloat("X", &light_position.x, -10.0f, 10.0f);
			ImGui::SliderFloat("Y", &light_position.y, -10.0f, 10.0f);
			ImGui::SliderFloat("Z", &light_position.z, -10.0f, 10.0f);
			ImGui::ColorEdit3("light ambient", (float*)&light_material.ambient);
			ImGui::ColorEdit3("light diffuse", (float*)&light_material.diffuse);
			ImGui::ColorEdit3("light specular", (float*)&light_material.speculer);

			ImGui::Spacing();
			ImGui::ColorEdit3("object ambient", (float*)&object_material.ambient);
			ImGui::ColorEdit3("object diffuse", (float*)&object_material.diffuse);
			ImGui::ColorEdit3("object specular", (float*)&object_material.speculer);
			// shininess
			ImGui::SetNextItemWidth(150);
			ImGui::SliderFloat("shininess", &object_material.shininess, 0.0f, 128.0f);

			ImGui::Spacing();
			ImGui::Text("lastX = %f", lastX);
			ImGui::Text("lastY = %f", lastY);

			ImGui::Text(
				"camera front = (%f,%f,%f) | up = (%f,%f,%f) | right = (%f,%f,%f)",
				camera.Front.x, camera.Front.y, camera.Front.z, camera.Up.x,
				camera.Up.y, camera.Up.z, camera.Position.x, camera.Position.y,
				camera.Position.z);

			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Render
		glfwSwapBuffers(window);

		// 解除绑定
		glBindVertexArray(0);
		//glBindBuffer(1, 0);

#ifdef _DEBUG
		if (0 != glGetError()) {
			spdlog::error("test error: {}", glGetError());
		}
#endif // DEBUG

	}

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

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
	glGenTextures(1, &textureID);
	glBindBuffer(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (auto i = 0; i < faces.size(); ++i) {
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, static_cast<void*>(data));
		}
		else {
			spdlog::error("Cubmap texture failed to load at path:{}", faces[i]);
		}
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	return textureID;

}

