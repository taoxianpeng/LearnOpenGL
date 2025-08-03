#pragma once

#include "application.h"
#include "node.h"
#include <memory>

class MyApplication : public Application
{
public:
    MyApplication() = default;
    ~MyApplication() = default;

    void onProjectLoad() override;
    void onUpdate() override;
    void onKeyInputEvent(const KeyEvent& keyEvent, const KeyActionEvent& keyActionEvent) override;

private:
    std::shared_ptr<Node> geometryNode;
    std::shared_ptr<Node> windowNode;  
    std::shared_ptr<Node> axioNode;  
    std::shared_ptr<Node> bigmanNode;  
    std::shared_ptr<Node> triangleNode;  
    std::shared_ptr<Node> instanceExNode;  
    std::shared_ptr<Node> skyBoxNode;  
};

Application* createApplication();