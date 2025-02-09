#include "car_model.h"
#include "log.h"
#include "direction.h"

void CarModel::loadData()
{
	const std::string resourcePath = Direction::getInstance()->getProjectRootPath().string();
	std::string vertPath = resourcePath + "/shader/color_shader.vert";
	std::string fragPath = resourcePath + "/shader/color_shader.frag";
	std::string modelPath = resourcePath + "/resource/car/bugatti.obj";

	LOGI("vertPath:{}", vertPath);
	LOGI("fragPath:{}", fragPath);
	LOGI("modelPath:{}", modelPath);


	m_shader = Shader(vertPath, fragPath);
	m_meshModel = MMesh::Model(modelPath);
}

void CarModel::draw()
{
	m_shader.use();
	m_shader.setMat4("view", m_view);
	m_shader.setMat4("projection", m_projection);
	m_model = glm::translate(m_model, glm::vec3(0.0f, 0.0f, 0.0f));
	m_model = glm::scale(m_model, glm::vec3(1.0f, 1.0f, 1.0f));
	m_shader.setMat4("model", m_model);
	m_shader.setVec3("light.position", m_lightPosition);
	m_shader.setVec3("light.diffuse", m_light.diffuse);
	m_shader.setVec3("viewPos", m_camera.Position);
	m_shader.setFloat("light.constant", 1.0f);
	m_shader.setFloat("light.linear", 0.09f);
	m_shader.setFloat("light.quadratic", 0.032f);

	m_meshModel.draw(m_shader);
}
