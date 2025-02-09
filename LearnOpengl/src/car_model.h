#pragma once
#include "node.h"

class CarModel : public Node
{
public:
  CarModel() = default;
  ~CarModel() = default;

  void loadData() override;
  void draw() override;

  void setLightPosition(const glm::vec3& position) {
    m_lightPosition = position;
  }

  const glm::vec3& getLightPositon() const {
    return m_lightPosition;
  }

  void setLightMaterial(const Light& light) {
    m_light = light;
  }

  const Light& getLightMaterial() const {
    return m_light;
  }

  void setCamera(const Camera& camera) {
    m_camera = camera;
  }

  const Camera& getCamera() const {
    return m_camera;
  }

private:
  Shader m_shader;
  MMesh::Model m_meshModel;
  glm::vec3 m_lightPosition;
  Camera m_camera;
  Light m_light;
};
