#pragma once

#include "glm/detail/qualifier.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/trigonometric.hpp"
#include <string>
#include <spdlog/spdlog.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "camera.h"
#include "common.h"
#include "mesh.h"
#include "texture.h"

#include <rttr/registration.h>

class Object
{
public:

  explicit Object() = default;
  virtual ~Object() = default;

  virtual void loadData() = 0;
  virtual void draw() = 0;
  
  RTTR_ENABLE();
};

class Node : public Object
{
public:
  explicit Node() = default;
  virtual ~Node() = default;

  void setName(const std::string& name) {
    m_name = name;
  }

  const std::string& getName() const {
    return m_name;
  }

  void setVisible(const bool& visible) {
    m_visible = visible;
  }

  const bool& getVisible() const {
    return m_visible;
  }

  virtual void loadData() override {};
  virtual void draw() override {};

  virtual void setView(const glm::mat4& view) {
    m_view = view;
  }

  virtual const glm::mat4& getView() const {
    return m_view;
  }

  virtual void setModel(const glm::mat4& model) {
    m_model = model;
  }

  virtual const glm::mat4& getModel() const{
    return m_model;
  }

  virtual void setProjection(const glm::mat4& project) {
    m_projection = project;
  }

  virtual const glm::mat4& getProjection() const {
    return m_projection;
  }

  void setTranslation(const glm::vec3& translation) {
    m_translation = translation;
    updateModelMat();
  }

  void setScale(const glm::vec3& scale) {
    m_scale = scale;
    updateModelMat();
  }

  void setRotation(const glm::vec3& rotation) {
    m_rotation = rotation;
    glm::quat quatX = glm::angleAxis(glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // X 轴旋转
    glm::quat quatY = glm::angleAxis(glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Y 轴旋转
    glm::quat quatZ = glm::angleAxis(glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Z 轴旋转
    m_rotationQuat = quatX * quatY * quatZ; // 组合旋转顺序：Y→X→Z
    updateModelMat();
  }

  const glm::vec3& getTranslation() const {
    return m_translation;
  }

  const glm::vec3& getScale() const {
    return m_scale;
  }

  const glm::vec3& getRotation() const {
    return m_rotation;
  }

  const GLuint getCubmapTextureID() const {
    return m_cubmapTextureID;
  }

  void setCubmapTextureID(GLuint cubmapTextureID) {
    m_cubmapTextureID = cubmapTextureID;
  }

  RTTR_ENABLE();

protected:
  glm::mat4 m_view;
  glm::mat4 m_model = glm::mat4(1.0f);
  glm::mat4 m_projection;

  glm::vec3 m_translation;
  glm::vec3 m_rotation;
  glm::quat m_rotationQuat;
  glm::vec3 m_scale = glm::vec3(1.0f, 1.0f, 1.0f);

  bool m_visible = false;
  std::string m_name;

  GLuint m_cubmapTextureID;

private:
  void updateModelMat() {
    m_model = glm::mat4(1.0f);
    m_model = glm::translate(m_model, m_translation);
    m_model = m_model * glm::mat4_cast(m_rotationQuat);
    m_model = glm::scale(m_model, m_scale);
    m_model = m_model;
  }
};