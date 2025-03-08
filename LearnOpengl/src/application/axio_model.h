#pragma once
#include "node.h"

class AxioModel : public Node {
public:
  AxioModel() = default;
  ~AxioModel() = default;
  void loadData() override;
  void draw() override;

  void setCamera(const Camera &camera) { m_camera = camera; }

  const Camera &getCamera() const { return m_camera; }

private:
  Shader m_shader;
  MMesh::Model m_meshModel;
  Camera m_camera;
};