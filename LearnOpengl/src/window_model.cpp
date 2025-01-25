#include "window_model.h"
#include "Shader.h"
#include "camera.h"
#include "log.h"
#include "direction.h"

void WindowModel::loadData() {
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

	const std::string& resourcePath = Direction::getInstance()->getProjectRootPath();
	std::string vertPath = resourcePath + "/shader/window.vert";
	std::string fragPath = resourcePath + "/shader/window.frag";

	LOGI("vertPath:{}", vertPath);
	LOGI("fragPath:{}", fragPath);


	m_shader = Shader(vertPath, fragPath);
    
}

void WindowModel::draw() {
    m_shader.use();
	m_shader.setMat4("view", m_view);
	m_shader.setMat4("projection", m_projection);
	m_model = glm::translate(m_model, glm::vec3(0.0f, 0.0f, 0.0f));
	m_model = glm::scale(m_model, glm::vec3(1.0f, 1.0f, 1.0f));
	m_shader.setMat4("model", m_model);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
}