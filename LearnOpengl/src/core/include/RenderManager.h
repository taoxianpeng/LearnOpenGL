#pragma once

#include "node.h"
#include <vector>
#include <memory>

class RenderManager {

  using NodeSPtr = std::shared_ptr<Node>;
  using NodeSPtrVec = std::vector<NodeSPtr>;

public:
  static RenderManager& getInstance() {
    static RenderManager renderManager;
    return renderManager;
  }

  NodeSPtrVec::iterator begin();
  NodeSPtrVec::iterator end();

  void loadResource();
  void drawAll();
  void addNode(const std::shared_ptr<Node>& node);
  void removeNode();

  void setSkyboxTextureID(GLuint id) {
      m_SkyboxTextureId = id;
  }

  GLuint getSkyBoxTextureID() {
    return m_SkyboxTextureId;
  }

  void setPV(const glm::mat4& projection, const glm::mat4& view);
  std::shared_ptr<Node>& findNode(const std::string& name);
  
private:
  RenderManager() = default;
  ~RenderManager() = default;

private:
  NodeSPtrVec m_RenderNodes;
  GLuint m_SkyboxTextureId;
};