#include "RenderManager.h"
#include <algorithm>
#include <memory>

void RenderManager::loadResource()
{
    for (const auto& node : m_RenderNodes) {
        if (node->getVisible()) {
            node->loadData();
        }
    }
}

void RenderManager::drawAll()
{
    for (const auto& node : m_RenderNodes) {
        if (node->getVisible()) {
            node->draw();
        }
    }
}

void RenderManager::addNode(const std::shared_ptr<Node>& node)
{
    m_RenderNodes.emplace_back(node);
}

void RenderManager::removeNode()
{
    // todo
}

std::shared_ptr<Node>& RenderManager::findNode(const std::string& name)
{
    for (auto& item : m_RenderNodes) {
        if (item->getName() == name) {
            return item;
        }
    }

    static std::shared_ptr<Node> emptyNode;
    return emptyNode;
}

void RenderManager::setPV(const glm::mat4& projection, const glm::mat4& view)
{
    for (auto& item : m_RenderNodes) {
        item->setProjection(projection);
        item->setView(view);
    }
}

RenderManager::NodeSPtrVec::iterator RenderManager::begin()
{
    return m_RenderNodes.begin();
}

RenderManager::NodeSPtrVec::iterator RenderManager::end()
{
    return m_RenderNodes.end();
}

