#pragma once

#include "node.h"
#include "Shader.h"
#include "camera.h"

class InstancingEx : public Node {
public:
  InstancingEx() = default;
  ~InstancingEx() = default;

  void loadData() override;
  void draw() override;

private:
  GLuint VAO;
  GLuint VBO;
  GLuint EBO;

  Shader m_shader;
  Texture m_texture;

  glm::vec2 translations[100];
};