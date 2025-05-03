#pragma once

#include "Instancing_ex.h"
#include "log.h"
#include "direction.h"

void InstancingEx::loadData()
{
  float quadVertices[] = {
    // λ��          // ��ɫ
    -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
     0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
    -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

    -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
     0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
     0.05f,  0.05f,  0.0f, 1.0f, 1.0f
  };

  int index = 0;
  float offset = 0.1f;
  for (int y = -10; y < 10; y += 2)
  {
    for (int x = -10; x < 10; x += 2)
    {
      glm::vec2 translation;
      translation.x = (float)x / 10.0f + offset;
      translation.y = (float)y / 10.0f + offset;
      translations[index++] = translation;
    }
  }

  CheckCall(glGenBuffers(1, &instanceVBO));
  CheckCall(glBindBuffer(GL_ARRAY_BUFFER, instanceVBO));
  CheckCall(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW));

  CheckCall(glGenVertexArrays(1, &VAO));
  CheckCall(glGenBuffers(1, &VBO));

  CheckCall(glBindVertexArray(VAO));
  CheckCall(glBindBuffer(GL_ARRAY_BUFFER,VBO));
  CheckCall(glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW));

  CheckCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
  CheckCall(glEnableVertexAttribArray(0));

  CheckCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float))));
  CheckCall(glEnableVertexAttribArray(1));


  CheckCall(glEnableVertexAttribArray(2));
  CheckCall(glBindBuffer(GL_ARRAY_BUFFER, instanceVBO));
  CheckCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0));

  CheckCall(glVertexAttribDivisor(2, 1));

  CheckCall(glBindVertexArray(0));
  CheckCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

  const std::string resourcePath = Direction::getInstance()->getProjectRootPath().string();
  std::string vertPath = resourcePath + "/shader/instancing.vert";
  std::string fragPath = resourcePath + "/shader/instancing.frag"; 

  LOGI("vertPath:{}", vertPath);
  LOGI("fragPath:{}", fragPath);

  m_shader = Shader(vertPath, fragPath);
}

void InstancingEx::draw() 
{
  m_shader.use();

  CheckCall(glBindVertexArray(VAO));
  // draw
  CheckCall(glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100));
  CheckCall(glBindVertexArray(0));

}
