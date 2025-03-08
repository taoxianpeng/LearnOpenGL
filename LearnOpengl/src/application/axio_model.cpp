#include "axio_model.h"
#include "log.h"
#include "direction.h"

void AxioModel::loadData()
{
	const std::string resourcePath = Direction::getInstance()->getProjectRootPath().string();
	std::string vertPath = resourcePath + "/shader/axio.vert";
	std::string fragPath = resourcePath + "/shader/axio.frag";
	std::string modelPath = resourcePath + "/resource/axio/axio.obj";

	LOGI("vertPath:{}", vertPath);
	LOGI("fragPath:{}", fragPath);
	LOGI("modelPath:{}", modelPath);


	m_shader = Shader(vertPath, fragPath);
	m_meshModel = MMesh::Model(modelPath);
	spdlog::debug("axio Shader class addr: {}", reinterpret_cast<uintptr_t>(&m_shader));
}

void AxioModel::draw()
{

  m_shader.use();
	m_shader.setMat4("view", m_view);
	m_shader.setMat4("projection", m_projection);
	m_model = glm::translate(m_model, glm::vec3(0.0f, 0.0f, 0.0f));
	m_model = glm::scale(m_model, glm::vec3(1.0f, 1.0f, 1.0f));
	m_shader.setMat4("model", m_model);

	m_meshModel.draw(m_shader);
}