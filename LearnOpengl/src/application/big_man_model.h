#pragma once
#include "glm/ext/vector_float3.hpp"
#include "node.h"
#include <rttr/registration.h>
#include <rttr/type.h>

class BigManModel : public Node
{
public:
  BigManModel() = default;
  ~BigManModel() = default;
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

  RTTR_ENABLE()

private:
  Shader m_shader;
  MMesh::Model m_meshModel;
  glm::vec3 m_lightPosition;
  Camera m_camera;
  Light m_light;
};

RTTR_REGISTRATION
{
    rttr::registration::class_<BigManModel>("BigMan")
        .constructor<>()
        .method("setTranslation", &BigManModel::setTranslation)(
          rttr::parameter_names("vec3"),
          rttr::default_arguments(glm::vec3(0.0f, 0.0f, 0.0f))
        )
        .method("getTranslation", &BigManModel::getTranslation)
        .method("setRotation", &BigManModel::setRotation)(
          rttr::parameter_names("vec3"),
          rttr::default_arguments(glm::vec3(0.0f, 0.0f, 0.0f))
        )
        .method("getRotation", &BigManModel::getRotation)
        .method("setScale", &BigManModel::setScale)(
          rttr::parameter_names("vec3"),
          rttr::default_arguments(glm::vec3(1.0f, 1.0f, 1.0f))
        )
        .method("getScale", &BigManModel::getScale);
}