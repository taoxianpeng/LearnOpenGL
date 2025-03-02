#pragma once

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

class Node
{
public:
  explicit Node() = default;
  virtual ~Node() = default;

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

protected:
  glm::mat4 m_view;
  glm::mat4 m_model;
  glm::mat4 m_projection;
};
