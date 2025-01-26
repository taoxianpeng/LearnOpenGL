#pragma once
#include "camera.h"
#include "node.h"

class TriangleModel : public Node {
public:
  TriangleModel() = default;
  ~TriangleModel();
  void loadData() override;
  void draw() override;

  void setCamera(const Camera &camera) { m_camera = camera; }

  const Camera &getCamera() const { return m_camera; }

private:
//   Shader m_shader;
  GLuint shaderProgram;

  MMesh::Model m_meshModel;
  Camera m_camera;

  GLuint VAO = 0;
  GLuint VBO = 0;
  GLuint EBO = 0;
};