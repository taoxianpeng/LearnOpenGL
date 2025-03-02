#include "geometry_ex.h"
#include "log.h"
#include "direction.h"

void GeometryEx::loadData()
{
	const std::string resourcePath = Direction::getInstance()->getProjectRootPath().string();
	std::string vertPath = resourcePath + "/shader/geometry_ex.vert";
	std::string fragPath = resourcePath + "/shader/geometry_ex.frag";
	std::string geomPath = resourcePath + "/shader/geometry_ex.geom";

	LOGI("vertPath:{}", vertPath);
	LOGI("fragPath:{}", fragPath);

  points = {
    -0.5f,  0.5f, // 左上
     0.5f,  0.5f, // 右上
     0.5f, -0.5f, // 右下
    -0.5f, -0.5f  // 左下
  };

  CheckCall(glGenVertexArrays(1, &VAO));
  CheckCall(glGenBuffers(1, &VBO));

  CheckCall(glBindVertexArray(VAO));
  CheckCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
  CheckCall(glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), points.data(), GL_STATIC_DRAW));

  CheckCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0)));
  CheckCall(glEnableVertexAttribArray(0));
  
  CheckCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
  CheckCall(glBindVertexArray(0));
  
	m_shader = Shader(vertPath, fragPath, geomPath);
}

void GeometryEx::draw()
{
  m_shader.use();
  CheckCall(glBindVertexArray(VAO));
  CheckCall(glDrawArrays(GL_POINTS, 0, 4));
}
