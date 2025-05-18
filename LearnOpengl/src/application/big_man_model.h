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

  void setLightAmbient(const glm::vec3& ambient) {
    m_light.ambient = ambient;
  }

  const glm::vec3& getLightAmbient() const {
    return m_light.ambient;
  }

  void setLightDiffuse(const glm::vec3& diffuse) {
    m_light.diffuse = diffuse;
  }

  const glm::vec3& getLightDiffuse() const {
    return m_light.diffuse;
  }

  void setLightSpeculer(const glm::vec3& speculer) {
    m_light.speculer = speculer;
  }

  const glm::vec3& getLightSpeculer() const {
    return m_light.speculer;
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
        .method("getScale", &BigManModel::getScale)
        .method("setVisible", &BigManModel::setVisible)(
          rttr::parameter_names("bool"),
          rttr::default_arguments(false)
        )
        .method("getVisible", &BigManModel::getVisible)
        .method("setLightPosition", &BigManModel::setLightPosition)(
          rttr::parameter_names("vec3"),
          rttr::default_arguments(glm::vec3(0.0f, 0.0f, 0.0f))
        )
        .method("getLightPosition", &BigManModel::getLightPositon)
        .method("setLightDiffuse", &BigManModel::setLightDiffuse)(
          rttr::parameter_names("vec3"),
          rttr::default_arguments(glm::vec3(0.0f, 0.0f, 0.0f))
        )
        .method("getLightDiffuse", &BigManModel::getLightDiffuse)
        .method("setLightAmbient", &BigManModel::setLightAmbient)(
          rttr::parameter_names("vec3"),
          rttr::default_arguments(glm::vec3(0.0f, 0.0f, 0.0f))
        )
        .method("getLightAmbient", &BigManModel::getLightAmbient)
        .method("setLightSpeculer", &BigManModel::setLightSpeculer)(
          rttr::parameter_names("vec3"),
          rttr::default_arguments(glm::vec3(0.0f, 0.0f, 0.0f))
        )
        .method("getLightSpeculer", &BigManModel::getLightSpeculer);
}