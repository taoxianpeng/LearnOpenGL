#pragma once

#include "node.h"
#include <vector>
#include <memory>

class RenderManager {
public:
  static RenderManager& getInstance() {
    static RenderManager renderManager;
    return renderManager;
  }

  void loadResource();
  void drawAll();
  void addNode(const std::shared_ptr<Node>& node);
  void removeNode();
  void setMPV(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model);
  std::shared_ptr<Node>& findNode(const std::string& name);
  
private:
  RenderManager() = default;
  ~RenderManager() = default;

private:
  std::vector<std::shared_ptr<Node>> m_RenderNodes;
};