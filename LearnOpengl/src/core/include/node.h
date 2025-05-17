#pragma once

#include "glm/detail/qualifier.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include <string>
#include <spdlog/spdlog.h>

#if WIN32
#include <glm.hpp>
#else
#include <glm/glm.hpp>
#endif

#include "camera.h"
#include "common.h"
#include "mesh.h"
#include "texture.h"

#include <rttr/registration.h>

class Node
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

  virtual void loadData() = 0;
  virtual void draw() = 0; 

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

  void setTransformation(const glm::vec3& transformation) {
    m_transformation = transformation;
    m_model = glm::translate(m_model, m_transformation);
  }

  void setScale(const glm::vec3& scale) {
    m_scale = scale;
    m_model = glm::scale(m_model, m_scale);
  }

  void setRotation(const glm::vec3& rotation) {
    m_rotation = rotation;
    // x axio rotate
    m_model = glm::rotate(m_model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    // y axio rotate
    m_model = glm::rotate(m_model, glm::radians(rotation.x), glm::vec3(0.0f, 1.0f, 0.0f));
    // z axio rotate
    m_model = glm::rotate(m_model, glm::radians(rotation.x), glm::vec3(0.0f, 0.0f, 1.0f));
  }

  const glm::vec3& getTransformation() const {
    return m_transformation;
  }

  const glm::vec3& getScale() const {
    return m_scale;
  }

  const glm::vec3& getRotation() const {
    return m_rotation;
  }

  RTTR_ENABLE();

protected:
  glm::mat4 m_view;
  glm::mat4 m_model;
  glm::mat4 m_projection;

  glm::vec3 m_transformation;
  glm::vec3 m_rotation;
  glm::vec3 m_scale;

  bool m_visible = false;
  std::string m_name;
};
